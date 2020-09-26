#include "OneSide.h"
#include "HardwareSerial.h"
#include "math.h"


OneSide::OneSide(){
    target_serial.begin(115200);
}
OneSide::~OneSide(){

}

float OneSide::__convert_command_from_toucher(){
    float toucher_length = toucher.get_meaning_value();
    // float THETA = acosf((540.0f * 540.0f + 265.0f * 265.0f - 550.0f * 550.0f) / (2.0f * 540.0f * 265.0f));
    // float ALPHA = acosf((88.0f * 88.0f + 193.0f * 193.0f -177.0f * 177.0f) / (2.0f * 88.0f * 193.0f));
    // float beta = acosf((88.0f * 88 + 193.0f * 193.0 - toucher_length * toucher_length ) / (2.0f * 88.0 * 193.0)) - ALPHA;
    // float target_length  = sqrt ( 540.0f * 540.0f + 265.0f * 265.0f - 2.0f * 540.0f * 265.0f * cosf(THETA - beta));
    float target_length = 415.0f + 235.0f * 145.0f / 60.0f - 145.0f / 60.0f * toucher_length;
    return target_length;

}
float OneSide::__get_error_length_of_actuaotr(){
    float target = __convert_command_from_toucher();
    return target - actuator_feedback.get_meaning_value();
}

void OneSide::set_show_debug(bool config){
    __config_show_debug = config;
}
void OneSide::__show_debug(){
    // Serial.print("read_sensors()  toucher adc = ");
    // Serial.print(toucher.get_last_reading());
    Serial.print("read_sensors()  toucher length = ");
    Serial.print(toucher.get_meaning_value());

    Serial.print ("  target_length = ");
    Serial.print (__target_length);

    // Serial.print("actuator feedback adc = ");
    // Serial.print(actuator_feedback.get_last_reading());
    Serial.print("  actuator feedback length = ");
    Serial.print (actuator_feedback.get_meaning_value());
    Serial.print("  error_length = ");
    Serial.print(__error_length);
    Serial.println(" ");
}

void OneSide::init(int side_id){
    __side_id = side_id;
}
void OneSide::output_curves(){

    if(__side_id == 1){
        int16_t target_length = __target_length;
        int16_t reality = target_length + __error_length;
        target_serial.write(0xff);
        target_serial.write(0xff);
        target_serial.write(0xff);
        target_serial.write(target_length / 256);
        target_serial.write(target_length % 256);
        target_serial.write(0xff);
        target_serial.write(reality / 256);
        target_serial.write(reality % 256);
    }

}
void OneSide::pid_loop_with_reading_sensors(){
    read_sensors();
    __target_length = __convert_command_from_toucher();
    __error_length =  actuator_feedback.get_meaning_value() - __target_length ;

    if(__config_show_debug) __show_debug();
    // return;

    // pid_loop
    if(__error_length > __error_max) __error_length = __error_max;
    if(__error_length < __error_min) __error_length = __error_min;

    pid_controller.UpdateError(__error_length);
    float output = pid_controller.get_output();
    pid_controller.show_errors_and_output();

    output_curves();
    // output pwm.
    uint16_t pwm_output = output * 32768 / 150 + 32768 ;   // 20mA == Longer length
    pwm_output = 65536 - pwm_output;

    // To protect actuaotr. 
    // When actuator reaches/close to end_point, but the commander want to move outside more,
    // in this scenario, we should stop actuator to move further. 
    bool endstop = false;  
    if (actuator_feedback.get_meaning_value() < 420  && __target_length < 410)
        endstop = true;
    if ( actuator_feedback.get_meaning_value() > 580 && __target_length > 580)
        endstop = true;
    if (endstop)
        pwm_output = 32768;   //output 12mA, means stop actuator.

    // Serial.print("PWM output = ");
    // Serial.println(pwm_output);

    actuator_pin.set_duty(pwm_output);
}

void OneSide::read_sensors(){
    toucher.read_adc();
    actuator_feedback.read_adc();
}

