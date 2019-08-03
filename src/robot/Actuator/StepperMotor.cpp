#include "StepperMotor.h"
#include "_hal/stopwatch.h"
#include "_hal/Pin/Pin.h"
// #include "_hal/Pin/OutputPin."


// #include "robot/StepTicker.h"   //??

#include <math.h>

StepperMotor::StepperMotor(OutputPin &step, OutputPin &dir, OutputPin &en):Actuator()
{
    __step_pin = OutputPin(step);
    __dir_pin = OutputPin(dir);
    __en_pin = OutputPin(en);

    if(en.connected()) {
        //set_high_on_debug(en.port_number, en.pin);
    }
    _motor_type = ACTUATOR_TYPE_T::STEPPER_MOTOR;
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
    current_position_steps += (direction?-1:1); return moving; 
}
//virtual override
void StepperMotor::unstep() { 
    __step_pin.set(0); 
}
//virtual override
void StepperMotor::set_direction(bool f) { 
    __dir_pin.set(f); 
    direction= f; 
}
//virtual override
void StepperMotor::enable(bool state)
{
    __en_pin.set(!state);
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
    if(this->direction != dir) {
        this->direction= dir;
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


