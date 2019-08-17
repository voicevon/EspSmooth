#include "Actuator.h"
#include "libs/StringUtils.h"
#include "HardwareSerial.h"
#include <math.h>

Actuator::Actuator()
{

}
Actuator::~Actuator(){

}
void Actuator::_init(){
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
    //static
Actuator::ACTUATOR_TYPE_T Actuator::get_type_from_string(const char* type_description){
    std::string upper = stringutils::toUpper(type_description);
    if(upper == "STEPPER") { return STEPPER_MOTOR; } 
    if(upper == "SERVO") { return SERVO_MOTOR; }
    if(upper == "XUEFENG") { return XUEFENG_MOTOR; }
    if(upper == "DC") { return DC_MOTOR; }

    
    Serial.print("[E][ActuatorType][from_string()] type_description= >");
    Serial.print(type_description);
    Serial.println("<    Forced to set as STEPPER_MOTOR");
    return STEPPER_MOTOR;  
}
void Actuator::change_steps_per_mm(float new_steps)
{
    steps_per_mm = new_steps;
    last_milestone_steps = roundf(last_milestone_mm * steps_per_mm);
    current_position_steps = last_milestone_steps;
}

void Actuator::change_last_milestone(float new_milestone)
{
    last_milestone_mm = new_milestone;
    last_milestone_steps = roundf(last_milestone_mm * steps_per_mm);
    current_position_steps = last_milestone_steps;
}

void Actuator::set_last_milestones(float mm, int32_t steps)
{
    last_milestone_mm= mm;
    last_milestone_steps= steps;
    current_position_steps= last_milestone_steps;
}

void Actuator::update_last_milestones(float mm, int32_t steps)
{
    last_milestone_steps += steps;
    last_milestone_mm = mm;
}

int32_t Actuator::steps_to_target(float target)
{
    int32_t target_steps = roundf(target * steps_per_mm);
    return target_steps - last_milestone_steps;
}

// Does a manual step pulse, used for direct encoder control of a stepper
// NOTE manual step is experimental and may change and/or be removed in the future, it is an unsupported feature.
// use at your own risk



