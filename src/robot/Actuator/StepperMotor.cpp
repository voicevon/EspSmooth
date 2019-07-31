#include "StepperMotor.h"
#include "robot/StepTicker.h"

#include <math.h>

StepperMotor::StepperMotor(Pin &step, Pin &dir, Pin &en):Actuator()
{
    step_pin = Pin(step);
    dir_pin = Pin(dir);
    en_pin = Pin(en);

    if(en.connected()) {
        //set_high_on_debug(en.port_number, en.pin);
    }

    steps_per_mm         = 1.0F;
    max_rate             = 50.0F;

    last_milestone_steps = 0;
    last_milestone_mm    = 0.0F;
    current_position_steps= 0;
    moving= false;
    acceleration= NAN;
    selected= true;
    extruder= false;

    enable(false);
    unstep(); // initialize step pin
    set_direction(false); // initialize dir pin
}

StepperMotor::~StepperMotor()
{
}


bool StepperMotor::step() { 
    step_pin.set(1); 
    current_position_steps += (direction?-1:1); return moving; 
}
void StepperMotor::unstep() { 
    step_pin.set(0); 
}

void StepperMotor::set_direction(bool f) { 
    dir_pin.set(f); 
    direction= f; 
}


