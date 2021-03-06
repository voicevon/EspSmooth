#include "FastTicker.h"
#include "stdio.h"

// #include "FreeRTOS.h"
// #include "freertos/task.h"

 #include "_hal/timer.h"
 #include "_hal/rtos.h"

// timers are specified in Hz and periods in microseconds
#define BASE_FREQUENCY 1000000
#define MAX_FREQUENCY 10000
#define MIN_FREQUENCY (FastTicker::get_min_frequency())


FastTicker *FastTicker::instance;

// This module uses a Timer to periodically call registered callbacks
// Modules register with a function ( callback ) and a frequency, and we then call that function at the given frequency.
// We could use tmr2 for this
FastTicker::FastTicker()
{
    instance= this;
}

FastTicker::~FastTicker()
{
    instance= nullptr;
    tmr2_stop();   
}

#define _ramfunc_ __attribute__ ((section(".ramfunctions"),long_call,noinline))

static void timer_handler()
{
    FastTicker::getInstance()->tick();
}

// called once to start the timer
bool FastTicker::start()
{
    if(max_frequency == 0) {
        //known consumers are:   Temperature control PWM output, in "config.frequency.pwm=10000"
        printf("WARNING: FastTicker not started as nothing has attached to it\n");
        return false;
    }

    if(!started) {
        if(max_frequency < MIN_FREQUENCY || max_frequency > MAX_FREQUENCY) {
            printf("ERROR: FastTicker cannot be set < %luHz or > %dHz\n", MIN_FREQUENCY, MAX_FREQUENCY);
            return false;
        }
        tmr2_setup(max_frequency, (void *)timer_handler);

    }else{
        printf("WARNING: FastTicker already started\n");
    }

    started= true;
    return true;
}

// called to stop the timer usually only called in TESTs
bool FastTicker::stop()
{
    if(started) {
        tmr2_stop();
        started= false;
    }
    return true;
}

int FastTicker::attach(uint32_t frequency, std::function<void(void)> cb)
{
    uint32_t period = BASE_FREQUENCY / frequency;
    int countdown = period;

    if( frequency > max_frequency ) {
        // reset frequency to a higher value
        if(!set_frequency(frequency)) {
            printf("ERROR: FastTicker cannot be set < %luHz or > %dHz\n", MIN_FREQUENCY, MAX_FREQUENCY);
            return -1;
        }
        max_frequency = frequency;
    }

    __taskENTER_CRITICAL();
    callbacks.push_back(std::make_tuple(countdown, period, cb));
    __taskEXIT_CRITICAL();

    // return the index it is in
    return callbacks.size()-1;
}

void FastTicker::detach(int n)
{
    // TODO need to remove it but that would change all the indexes
    // For now we just zero the callback
    __taskENTER_CRITICAL();
    std::get<2>(callbacks[n])= nullptr;
    __taskEXIT_CRITICAL();
}

// Set the base frequency we use for all sub-frequencies
// NOTE this is a fast ticker so ticks slower than 1000Hz are not allowed
bool FastTicker::set_frequency( int frequency )
{
    if(frequency < (int)MIN_FREQUENCY || frequency > MAX_FREQUENCY) return false;
    this->interval = BASE_FREQUENCY / frequency; // microsecond period

    if(started) {
        // change frequency of timer callback
        tmr2_set_frequency(frequency);
    }

    return true;
}

// This is an ISR anything that this calls that is faster than 1KHz should be a ramfunc too
void FastTicker::tick()
{
    // Call all callbacks that need to be called
    for(auto& i : callbacks) {
        int& countdown= std::get<0>(i);
        countdown -= this->interval;
        if (countdown < 0) {
            countdown += std::get<1>(i);
            auto& fnc= std::get<2>(i); // get the callback
            if(fnc) {
                fnc();
            }
        }
    }
}
