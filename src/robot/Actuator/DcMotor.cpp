#include "DcMotor.h"

const int SERVO_FREQ = 250;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution

static uint8_t pwm_channel = 0;

DcMotor::DcMotor(Pin dir, Pin pwm){
    __pin_dir = dir;
    __pin_pwm = pwm;
    __pwm_channel = pwm_channel;
    ledcSetup(pwm_channel, SERVO_FREQ, PWM_RESOLUTION_BITS);
    ledcAttachPin(__pin_pwm.get_gpiopin(), __pwm_channel);
    ledcWrite(__pwm_channel, 0);

    // my goal: 
    //__pwm_channel =  __pin_pwm.setup_pwm(frequency,resolution,init_value = 0); 

}

bool DcMotor::step(){
    // __pwm_channel.update_pwm(this->get_current_position());
    return true;
}


const int MIN_PULSE = 1000; // 
const int MAX_PULSE = 75000; //



