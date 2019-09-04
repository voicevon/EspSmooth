#include "OneSide.h"


void OneSide::init(AdcPin toucher_pin,AdcPin feedback_pin, PwmPin actuator_pin,float pid_p,float pid_i,float pid_d){
    __pid_controller.Init(pid_p,pid_i,pid_d);
    toucher_pin.start();
    feedback_pin.start();
    actuator_pin.start();
}

void OneSide::pid_loop_with_reading_sensors(){
    read_sensors();
    // pid_loop
    __pid_controller.UpdateError(__error_length);
    float output = __pid_controller.OutputSteerAng();
    // output pwm
    actuator_pin.set_duty(output);
}

void OneSide::read_sensors(){
    float toucher_adc = 1;
    float feedback_adc =12;

    __toucher_length = __toucher_to_length(toucher_adc);
    __feedback_length = __feedback_to_length(feedback_adc);

    __error_length = __feedback_length - __toucher_length;
}

float OneSide::__toucher_to_length(float toucher_adc){
    return 1;
}
float OneSide::__feedback_to_length(float feedback_adc){
    return 2;
}