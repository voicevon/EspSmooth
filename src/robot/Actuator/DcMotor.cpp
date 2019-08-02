#include "DcMotor.h"
#include "_hal/Pin/PinHelper.h"

const int PWM_FREQ = 250;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution


DcMotor::DcMotor(OutputPin& dir_pin, PwmPin& pwm_pin){
    __dir_pin = OutputPin(dir_pin);
    __pwm_pin = PwmPin(pwm_pin);
}

bool DcMotor::step(){
    // pwm_pin.set_value(this->get_current_position());
    return true;
}

//virtual  override
void DcMotor::enable(bool state){
    if(state){
        __dir_pin.start();
        __pwm_pin.start();
    } else {
        __dir_pin.stop();
        __pwm_pin.stop();
    }
}


const int MIN_PULSE = 1000; // 
const int MAX_PULSE = 75000; //



