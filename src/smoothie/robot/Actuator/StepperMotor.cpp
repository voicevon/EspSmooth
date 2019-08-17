#include "StepperMotor.h"
#include "_hal/stopwatch.h"
#include "_hal/Pin/Pin.h"
// #include "_hal/Pin/OutputPin."


// #include "robot/StepTicker.h"   //??

#include <math.h>

#include "HardwareSerial.h"

StepperMotor::StepperMotor(OutputPin &step, OutputPin &dir, OutputPin &en):Actuator()
{
    motor_type_ = ACTUATOR_TYPE_T::STEPPER_MOTOR;
    __step_pin = OutputPin(step);
    __dir_pin = OutputPin(dir);
    __en_pin = OutputPin(en);

    if(en.connected()) {
        //set_high_on_debug(en.port_number, en.pin);
    }
    _init();
    // steps_per_mm         = 1.0F;
    // max_rate             = 50.0F;

    // last_milestone_steps = 0;
    // last_milestone_mm    = 0.0F;
    // current_position_steps= 0;
    // moving= false;
    // acceleration= NAN;
    // selected= true;
    // extruder= false;

    // enable(false);
    // unstep(); // initialize step pin
    // set_direction(false); // initialize dir pin
}

StepperMotor::~StepperMotor()
{
}

//virtual override
bool StepperMotor::step() { 
    __step_pin.set(1); 
    current_position_steps += (direction_ ? -1 : 1); return moving; 
}
//virtual override
void StepperMotor::unstep() { 
    __step_pin.set(0); 
}
//virtual override
void StepperMotor::set_direction(bool new_direction) { 
    // Serial.print("[V][StepperMotor] set_direction() = ");
    // Serial.println(new_direction);

    __dir_pin.set(new_direction); 
    // Serial.println("[V][StepperMotor] set_direction() bbbbbbbbbbbbbbb ");
    direction_ = new_direction; 
    // Serial.println("[V][StepperMotor] set_direction() eeeeeeeeeeeeeee ");
}
//virtual override
void StepperMotor::enable(bool state)
{
    // printf("[V][StepperMotor] enable() state= %i\n",state);
    __en_pin.set(state);
}
//virtual override
bool StepperMotor::is_enabled() const
{
    return !__en_pin.get();
}
//virtual override
void StepperMotor::manual_step(bool dir)
{
    if(!is_enabled()) enable(true);

    // set direction if needed
    if(this->direction_ != dir) {
        this->direction_ = dir;
        this->__dir_pin.set(dir);
        StopWatch_DelayUs(1);
    }

    // pulse step pin
    this->__step_pin.set(1);
    StopWatch_DelayUs(3); // TODO could use configured step pulse delay
    this->__step_pin.set(0);

    // keep track of actuators actual position in steps
    this->current_position_steps += (dir ? -1 : 1);
}



#define BOARD_PRIMEALPHA
#ifdef BOARD_PRIMEALPHA
// prime Alpha has TMC2660 drivers so this handles the setup of those drivers
#include "smoothie/robot/drivers/TMC26X.h"

bool StepperMotor::vmot= false;
bool StepperMotor::setup_tmc2660(ConfigReader& cr, const char *actuator_name)
{
    char axis= motor_id<3?'X'+motor_id:'A'+motor_id-3;
    printf("DEBUG: setting up tmc2660 for %s, axis %c\n", actuator_name, axis);
    tmc2660= new TMC26X(axis);
    if(!tmc2660->config(cr, actuator_name)) {
        delete tmc2660;
        return false;
    }
    tmc2660->init();

    return true;
}

bool StepperMotor::init_tmc2660()
{
    if(tmc2660 == nullptr) return false;
    tmc2660->init();
    return true;
}

bool StepperMotor::set_current(float c)
{
    if(tmc2660 == nullptr) return false;
    // send current to TMC2660
    tmc2660->setCurrent(c*1000.0F); // sets current in milliamps
    return true;
}

bool StepperMotor::set_microsteps(uint16_t ms)
{
    if(tmc2660 == nullptr) return false;
    tmc2660->setMicrosteps(ms); // sets microsteps
    return true;
}

int StepperMotor::get_microsteps()
{
    if(tmc2660 == nullptr) return 0;
    return tmc2660->getMicrosteps();
}



void StepperMotor::dump_status(OutputStream& os, bool flag)
{
    if(tmc2660 == nullptr) return;
    tmc2660->dump_status(os, flag);
}

void StepperMotor::set_raw_register(OutputStream& os, uint32_t reg, uint32_t val)
{
    if(tmc2660 == nullptr) return;
    tmc2660->set_raw_register(os, reg, val);
}

bool StepperMotor::set_options(GCode& gcode)
{
    if(tmc2660 == nullptr) return false;
    return tmc2660->set_options(gcode);
}

bool StepperMotor::check_driver_error()
{
    if(tmc2660 == nullptr) return false;
    return tmc2660->check_errors();
}

#else

//Minialpha has enable pins on the drivers
void StepperMotor::enable(bool state)
{
    if(tmc2660 == nullptr) {
        if(en_pin.connected()) {
            en_pin.set(!state);
        }
        return;
    }

    if(state && !vmot){
        //printf("WARNING: %d: trying to enable motors when vmotor is off\n", motor_id);
        if(is_enabled())
            tmc2660->setEnabled(false);
        return;
    }

    // if we have lost Vmotor since last time then we need to re load all the drivers configs
    if(state && vmot_lost) {
        if(vmot) {
            tmc2660->init();
            tmc2660->setEnabled(true);
            vmot_lost= false;
            printf("DEBUG: tmc2660: %d inited\n", motor_id);
        }else{
            tmc2660->setEnabled(false);
        }
        return;
    }

    // we don't want to enable/disable it if it is already in that state to avoid sending SPI all the time
    bool en= is_enabled();
    if((!en && state) || (en && !state)) {
        tmc2660->setEnabled(state);
    }
}

bool StepperMotor::is_enabled() const
{
    if(tmc2660 == nullptr) {
        if(en_pin.connected()) {
            return !en_pin.get();
        }
        // presume always enabled
        return true;
    }
    return tmc2660->isEnabled();
}


#endif
