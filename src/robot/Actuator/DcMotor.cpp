#include "DcMotor.h"
#include "_hal/Pin/PinHelper.h"

const int SERVO_FREQ = 250;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution

static uint8_t pwm_channel = 0;

DcMotor::DcMotor(Pin dir, Pin pwm){
    __pin_dir = dir;
    __pin_pwm = pwm;
    __pwm_channel = pwm_channel;
    ledcSetup(pwm_channel, SERVO_FREQ, PWM_RESOLUTION_BITS);
    ledcAttachPin(__pin_pwm.get_gpio_num(), __pwm_channel);
    ledcWrite(__pwm_channel, 0);

    // my goal: 
    //__pwm_channel =  __pin_pwm.setup_pwm(frequency,resolution,init_value = 0); 

}
//My goal B:
DcMotor::DcMotor(OutputPin dir_pin, PwmPin pwm_pin){
    // dir_pin.init();

    // pwm_pin.init(channel=auto_get,frequency = 500);
    // pwm_pin.init(channel=auto_get,frequency = 500, min = 10, max = 359);
    // pwm_pin.init(channel=auto_get,frequency = 500, min = 10, max = 359, start_pwm_value = 10);

    // pwm_pin.start(pwm_value = 1234);
    // pwm_pin.stop();   //set as input;
}

bool DcMotor::step(){
    // pwm_pin.set_value(this->get_current_position());
    return true;
}


const int MIN_PULSE = 1000; // 
const int MAX_PULSE = 75000; //



