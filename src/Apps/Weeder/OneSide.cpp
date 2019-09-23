#include "OneSide.h"
#include "HardwareSerial.h"


OneSide::OneSide(){
}
OneSide::~OneSide(){

}

// void OneSide::init_io(AdcPin _toucher_pin,AdcPin _feedback_pin, PwmPin _actuator_pin){
//     this->toucher_pin = _toucher_pin;
//     this->actuator_feedback_pin = _feedback_pin;
//     this->actuator_pin = _actuator_pin;

//     this->toucher_pin.start();
//     this->actuator_feedback_pin.start();
//     this->actuator_pin.init(5000, 16, 0);
//     this->actuator_pin.start();
// }

// void OneSide::init_pid(float pid_p,float pid_i,float pid_d){
//     __pid_controller.Init(pid_p,pid_i,pid_d);
// }
// void OneSide::init_toucher(AdcPin toucher_pin,float slope, float offset){
//    // __toucher_slope = 0.02061;
//     // __toucher_offset = 177.0
//     toucher.init(toucher_pin,)
// }
// void OneSide::init_actuator_sensor(AdcPin feedback_sensor_pin,float slope, float offset){

// }
// void OneSide::init_actuator(PwmPin actuator_pin){

// }

#include "math.h"
float OneSide::__convert_command_from_toucher(){
    float toucher_length = toucher.get_meaning_value();
    float THETA = acosf((540.0f * 540.0f + 265.0f * 265.0f - 550.0f * 550.0f) / (2.0f * 540.0f * 265.0f));
    float ALPHA = acosf((88.0f * 88.0f + 193.0f * 193.0f -177.0f * 177.0f) / (2.0f * 88.0f * 193.0f));
    float beta = acosf((88.0f * 88 + 193.0f * 193.0 - toucher_length * toucher_length ) / (2.0f * 88.0 * 193.0)) - ALPHA;
    float target_length  = sqrt ( 540.0f * 540.0f + 265.0f * 265.0f - 2.0f * 540.0f * 265.0f * cosf(THETA - beta));

    return target_length;

}
float OneSide::__get_error_length_of_actuaotr(){
    float target = __convert_command_from_toucher();
    return target - actuator_feedback.get_meaning_value();
}
void OneSide::pid_loop_with_reading_sensors(){
    read_sensors();
    float target_length = __convert_command_from_toucher();
    __error_length =  actuator_feedback.get_meaning_value() - target_length ;
    Serial.print("read_sensors()  toucher length = ");
    Serial.print(toucher.get_meaning_value());
    Serial.print ("  target_length = ");
    Serial.print (target_length);
    Serial.print("  actuator feedback length = ");
    Serial.print (actuator_feedback.get_meaning_value());
    Serial.print(" error_length = ");
    Serial.println(__error_length);

    // pid_loop
    pid_controller.UpdateError(__error_length);
    float output = pid_controller.OutputSteerAng();
    // output pwm

    uint16_t pwm_output = toucher.get_meaning_value() * 65536.0 / 61.0  - 177.0f * 65536.0f / 61.0f  ;
    Serial.print("PWM output = ");
    Serial.println(pwm_output);
    actuator_pin.set_duty(pwm_output);
}

void OneSide::read_sensors(){
    toucher.read_adc();
    actuator_feedback.read_adc();
}

