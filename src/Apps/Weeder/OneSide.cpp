#include "OneSide.h"
#include "HardwareSerial.h"
#include "math.h"


OneSide::OneSide(){
}
OneSide::~OneSide(){

}

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
    // uint16_t pwm_output = - output * 32768 / 300 + 32768 ;   // 20mA == Shorter length
    // uint16_t pwm_output = output * 32768 / 150 + 32768 ;   // 20mA == Longer length
    Serial.print("PWM output = ");
    Serial.println(pwm_output);
    actuator_pin.set_duty(pwm_output);
}

void OneSide::read_sensors(){
    toucher.read_adc();
    actuator_feedback.read_adc();
}

