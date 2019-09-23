#include "PwmActuator.h"

PwmActuator::PwmActuator(PwmPin pwm_pin,int frequency, int resolution,int default_value){

}
void PwmActuator::attatch_feedback_sensor(Sensor* sensor){
    __feedback_sensor = sensor;
}
void PwmActuator::start(){

}
float PwmActuator::get_error(){
    return 0;
}

 
