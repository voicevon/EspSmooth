#include "StepTicker.h"

#include "smoothie/smoothie/AxisDefns.h"
#include "Actuator/StepperMotor.h"
#include "Block.h"
#include "Conveyor.h"
#include "smoothie/smoothie/Module.h"
// #include "tmr-setup.h"
#include "_hal/timer.h"

#include <fcntl.h>
#include <errno.h>

#include <math.h>

#include "HardwareSerial.h"

#ifdef STEPTICKER_DEBUG_PIN
// debug pins, only used if defined in src/makefile
#include "Pin.h"
Pin stepticker_debug_pin(STEPTICKER_DEBUG_PIN, Pin::AS_OUTPUT);
#define SET_STEPTICKER_DEBUG_PIN(n) { stepticker_debug_pin.set(n); }
#else
#define SET_STEPTICKER_DEBUG_PIN(n)
#endif

// TODO move ramfunc define to a utils.h
#define _ramfunc_ __attribute__ ((section(".ramfunctions"),long_call,noinline))

StepTicker *StepTicker::instance= nullptr;

StepTicker::StepTicker()
{
    if(instance == nullptr) instance= this;
}

StepTicker::~StepTicker()
{
}

// ISR callbacks from timer
void StepTicker::step_timer_handler(void)
{
    // Serial.print(">");
    StepTicker::getInstance()->step_tick();
}

// ISR callbacks from timer
void StepTicker::unstep_timer_handler(void)
{
    Serial.print("<");
    StepTicker::getInstance()->unstep_tick();
}

//TODO:  will use TIMER1 because TIMER0 is used by RTOS.   Xuming Jun 2019
bool StepTicker::start()
{
    if(!started) {

        // setup the step tick timer, which handles step ticks and one off unstep interrupts
        int permod = stepTicker_setup(frequency, delay, (void *)step_timer_handler, (void *)unstep_timer_handler);
        if(permod <  0) {
            printf("ERROR: tmr1 setup failed\n");
            return false;
        }
        if(permod != 0) {
            printf("Warning: stepticker is not accurate: %d\n", permod);
            // TODO adjust actual step frequency accordingly
        }

        started = true;
    }

    current_tick = 0;

    return true;
}

bool StepTicker::stop()
{
    if(started) {
        stepTicker_stop();
    }
    return true;
}

// Set the base stepping frequency
// can only be set before it is started
void StepTicker::set_frequency( float freq )
{
    if(started) {
        printf("ERROR: cannot set stepticker frequency after it has been started\n");
        return;
    }

    this->frequency = floorf(freq);
}

// Set the reset delay, must be called before started
void StepTicker::set_unstep_time( float microseconds )
{
    if(started) {
        printf("ERROR: cannot set stepticker unstep delay after it has been started\n");
        return;
    }

    // check that the unstep time is less than the step period
    uint32_t d= roundf(microseconds);
    uint32_t period= floorf(1000000.0F/frequency);
    if(d > period-1) { // within 1us of the period
        printf("ERROR: cannot set stepticker unstep delay greater than or equal to step ticker period: %llu, %llu\n", delay, period);
        return;
    }

    delay = d;
}

bool StepTicker::start_unstep_ticker()
{
    // enable the MR1 match register interrupt
    // this works as we are in MR0 match which reset counter so we will get an interrupt 2us after this is enabled
    // which we will use to unstep the step pin.
    tmr1_mr1_start();
    return true;
}

// Reset step pins on any motor that was stepped
void StepTicker::unstep_tick()
{
    uint32_t bitmsk= 1;
    for (int i = 0; i < num_motors; i++) {
        if(this->unstep & bitmsk) {
            this->motor[i]->unstep();
            // Serial.print("\\");
        }
        bitmsk <<= 1;
    }
    this->unstep= 0;
}

// extern "C" void PendSV_Handler(void)
// {
//     StepTicker::getInstance()->handle_finish();
// }

// slightly lower priority than TIMER0, the whole end of block/start of block is done here allowing the timer to continue ticking
// void StepTicker::handle_finish (void)
// {
//     // all moves finished signal block is finished
//     if(finished_fnc) finished_fnc();
// }

// step clock
void StepTicker::step_tick (void)
{
    // Serial.print(xPortGetCoreID());
    // if nothing has been setup we ignore the ticks
    if(!running) {
        // check if anything new available
        if(Conveyor::getInstance()->get_next_block(&current_block)) { // returns false if no new block is available
            // Serial.println("[V][StepTicker] step_tick() aaaaaaaaaaaaaa .");
            running = start_next_block(); // returns true if there is at least one motor with steps to issue
            // Serial.println("[V][StepTicker] step_tick() bbbbbbbbbbbbbb .");
            if(!running){ 
                Serial.println("[W][StepTicker] step_tick() robot is not running.");
                return;
            }
        } else {
            // Serial.println("[W][StepTicker] step_tick() no new block is aavailable.");
            return;
        }
    }
    if(Module::is_halted()) {
        running = false;
        current_tick = 0;
        current_block = nullptr;
        return;
    }
    bool still_moving = false;
    // foreach motor, if it is active see if time to issue a step to that motor
    for (uint8_t m = 0; m < num_motors; m++) {
        if(current_block->tick_info[m].steps_to_move == 0) continue; // not active
        current_block->tick_info[m].steps_per_tick += current_block->tick_info[m].acceleration_change;

        if(current_tick == current_block->tick_info[m].next_accel_event) {
            if(current_tick == current_block->accelerate_until) { 
                // We are done accelerating, deceleration becomes 0 : plateau
                current_block->tick_info[m].acceleration_change = 0;
                if(current_block->decelerate_after < current_block->total_move_ticks) {
                    current_block->tick_info[m].next_accel_event = current_block->decelerate_after;
                    if(current_tick != current_block->decelerate_after) { // We are plateauing
                        // steps/sec / tick frequency to get steps per tick
                        current_block->tick_info[m].steps_per_tick = current_block->tick_info[m].plateau_rate;
                    }
                }
            }

            if(current_tick == current_block->decelerate_after) { // We start decelerating
                current_block->tick_info[m].acceleration_change = current_block->tick_info[m].deceleration_change;
            }
        }

        // protect against rounding errors and such
        if(current_block->tick_info[m].steps_per_tick <= 0) {
            current_block->tick_info[m].counter = STEPTICKER_FPSCALE; // we force completion this step by setting to 1.0
            current_block->tick_info[m].steps_per_tick = 0;
        }

        current_block->tick_info[m].counter += current_block->tick_info[m].steps_per_tick;

        if(current_block->tick_info[m].counter >= STEPTICKER_FPSCALE) { // >= 1.0 step time
            current_block->tick_info[m].counter -= STEPTICKER_FPSCALE; // -= 1.0F;
            ++current_block->tick_info[m].step_count;

            // step the motor
            // if(m==1){
            //     Serial.print("^");    // Even can we find one sign?
            // }
            bool ismoving = motor[m]->step(); // returns false if the moving flag was set to false externally (probes, endstops etc)
            // Solution A: Easier, Simpler, but Stupit
            // delayMicroseconds(1);   // 100ns is enough
            this->motor[m]->unstep();
            // Solution B:we stepped so schedule an unstep
            // unstep |= (1<<m);

            if(!ismoving || current_block->tick_info[m].step_count == current_block->tick_info[m].steps_to_move) {
                // done
                current_block->tick_info[m].steps_to_move = 0;
                motor[m]->stop_moving(); // let motor know it is no longer moving
                // Serial.println("[V][StepTicker]::step_tick() Block is done, to stop motor.");        // Even can we find one sign?
            }
        }

        // see if any motors are still moving after this tick
        if(motor[m]->is_moving()) still_moving = true;
    }

    // do this after so we start at tick 0
    current_tick++; // count number of ticks

    // We may have set a pin on in this tick, now we set the timer to set it off
    // right now it takes about 1-2us to get here which will add to the pulse width from when it was on
    // the pulse width will be 1us (or whatever it is set to) from this point on, so at least 2-3 us
    if( unstep != 0) {
        start_unstep_ticker();
    }

    // see if any motors are still moving
    if(!still_moving) {
        //SET_STEPTICKER_DEBUG_PIN(0);
        //Two reason be here:  Not started the first moving , or finished the previous moving.
        // all moves finished
        current_tick = 0;

        // get next block
        // do it here so there is no delay in ticks
        Conveyor::getInstance()->block_finished();
        Serial.println("ISR\n\n\n");

        if(Conveyor::getInstance()->get_next_block(&current_block)) { // returns false if no new block is available
            Serial.println("[D][StepTicker]::step_tick() going to start_next_block()");
            running = start_next_block(); // returns true if there is at least one motor with steps to issue

        } else {
            Serial.println("[D][StepTicker]::step_tick() next_block is empty()");
            current_block = nullptr;
            running = false;
        }
        // all moves finished
        // we delegate the slow stuff to the pendsv handler which will run as soon as this interrupt exits
        //NVIC_SetPendingIRQ(PendSV_IRQn); this doesn't work
        //SCB->ICSR = 0x10000000; // SCB_ICSR_PENDSVSET_Msk;
    }
}

// only called from the step tick ISR (single consumer)
bool StepTicker::start_next_block()
{
    // Serial.println("[V][StepTicker] start_next_block() aaaaaaaaaaaaaaaaa");
    if(current_block == nullptr) return false;
    // Serial.println("[V][StepTicker] start_next_block() bbbbbbbbbbbbbbbbbbb");
    bool ok = false;
    // need to prepare each active motor
    for (uint8_t m = 0; m < num_motors; m++) {
        // Serial.print("[V][StepTicker] start_next_block() m= ");
        // Serial.println(m);

        if(current_block->tick_info[m].steps_to_move == 0) continue;
        // Serial.println("[V][StepTicker] start_next_block() ccccccccccccc");

        ok = true; // mark at least one motor is moving
        // set direction bit here
        // NOTE this would be at least 10us before first step pulse.
        // TODO does this need to be done sooner, if so how without delaying next tick
        // Serial.println("[V][StepTicker] start_next_block() ddddddddddddddd");
        motor[m]->set_direction(current_block->direction_bits[m]);
        // Serial.println("[V][StepTicker] start_next_block() fffffffffffffff");

        motor[m]->start_moving(); // also let motor know it is moving now
        // Serial.println("[V][StepTicker] start_next_block() hhhhhhhhhhhhhhhh");

    }
    // Serial.println("[V][StepTicker] start_next_block() jjjjjjjjjjjjjjjjjj");

    current_tick = 0;

    if(ok) {
        //SET_STEPTICKER_DEBUG_PIN(1);
        return true;

    } else {
        // this is an edge condition that should never happen, but we need to discard this block if it ever does
        // basically it is a block that has zero steps for all motors
        Conveyor::getInstance()->block_finished();
    }
    // Serial.println("[V][StepTicker] start_next_block() kkkkkkkkkkkkkkkkkkkk");

    return false;
}

// returns index of the stepper motor in the array and bitset
int StepTicker::register_actuator(Actuator* m)
{
    motor[num_motors++] = m;
    return num_motors - 1;
}

