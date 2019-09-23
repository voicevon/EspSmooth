#include "OneSide.h"
#include "HardwareSerial.h"


OneSide::OneSide(){
}
OneSide::~OneSide(){

}

void OneSide::init(AdcPin _toucher_pin,AdcPin _feedback_pin, PwmPin _actuator_pin,float pid_p,float pid_i,float pid_d){
    __pid_controller.Init(pid_p,pid_i,pid_d);

    this->toucher_pin = _toucher_pin;
    this->actuator_feedback_pin = _feedback_pin;
    this->actuator_pin = _actuator_pin;

    this->toucher_pin.start();
    this->actuator_feedback_pin.start();
    this->actuator_pin.init(5000, 16, 0);
    this->actuator_pin.start();
}
void OneSide::init(){
}
#include "math.h"
float OneSide::__convert_command_from_toucher(){
    float toucher_length = this->get_toucher_length();
    float THETA = acosf((540.0f * 540.0f + 265.0f * 265.0f - 550.0f * 550.0f) / (2.0f * 540.0f * 265.0f));
    float ALPHA = acosf((88.0f * 88.0f + 193.0f * 193.0f -177.0f * 177.0f) / (2.0f * 88.0f * 193.0f));
    float beta = acosf((88.0f * 88 + 193.0f * 193.0 - toucher_length * toucher_length ) / (2.0f * 88.0 * 193.0)) - ALPHA;
    float target_length  = sqrt ( 540.0f * 540.0f + 265.0f * 265.0f - 2.0f * 540.0f * 265.0f * cosf(THETA - beta));

    return target_length;

}
float OneSide::__get_error_length_of_actuaotr(){
    float target = __convert_command_from_toucher();
    float real_length = this->__feedback_length;
    return target -real_length;
}
void OneSide::pid_loop_with_reading_sensors(){
    read_sensors();
    // __error_length = __get_error_length_of_actuaotr();
    float target_length = __convert_command_from_toucher();
    float real_length = this->__feedback_length;
    __error_length =  real_length - target_length ;
    Serial.print("read_sensors()  toucher length = ");
    Serial.print(__toucher_length);
    Serial.print ("  target_length = ");
    Serial.print (target_length);
    Serial.print("  actuator feedback length = ");
    Serial.print (__feedback_length);
    Serial.print(" error_length = ");
    Serial.println(__error_length);

    // pid_loop
    __pid_controller.UpdateError(__error_length);
    float output = __pid_controller.OutputSteerAng();
    // output pwm

    uint16_t pwm_output = __toucher_length * 65536.0 / 61.0  - 177.0f * 65536.0f / 61.0f  ;
    Serial.print("PWM output = ");
    Serial.println(pwm_output);
    actuator_pin.set_duty(pwm_output);
}

void OneSide::read_sensors(){
    float toucher_adc = toucher_pin.read();
    float feedback_adc = actuator_feedback_pin.read();

    __toucher_length = __toucher_to_length(toucher_adc);
    __feedback_length = __feedback_to_length(feedback_adc);

    // __error_length = __feedback_length - __toucher_length;

}

float OneSide::__toucher_to_length(float toucher_adc){

    __toucher_slope = 0.02061;
    __toucher_offset = 177.0;
    // Serial.print("toucher original adc = ");
    // Serial.println(toucher_adc);
    float result =  __toucher_slope * toucher_adc + __toucher_offset;
    // Serial.println(result);

    return result;
}
float OneSide::__feedback_to_length(float feedback_adc){
    __feedback_slope = 0.04115;
    __feedback_offset = 413.0;
    // Serial.print("feedback original adc = ");
    // Serial.println(feedback_adc);
    float result =  __feedback_slope * feedback_adc + __feedback_offset;
    // Serial.println(result);

    return result;
}