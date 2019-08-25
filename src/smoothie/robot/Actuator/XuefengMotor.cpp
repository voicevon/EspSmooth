# include "XuefengMotor.h"


XuefengMotor::XuefengMotor():Actuator(){
    motor_type_ = ACTUATOR_TYPE_T::XUEFENG_MOTOR;
    
}

void XuefengMotor::goto_position(float target_position){

}
bool XuefengMotor::step() {
    current_position_steps_ += (direction_ ? -1 : 1); 
    return moving_; 
}

