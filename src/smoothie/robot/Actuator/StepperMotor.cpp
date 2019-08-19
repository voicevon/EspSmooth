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
    current_position_steps_ += (direction_ ? -1 : 1); 
    return moving_; 
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
    if(__en_pin.is_valid()){
        __en_pin.set(state);
        enabled_ = true;
    }else{
        enabled_ = false;
    }
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
    this->current_position_steps_ += (dir ? -1 : 1);
}


