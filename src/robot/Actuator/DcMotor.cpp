#include "DcMotor.h"
#include "_hal/Pin/PinHelper.h"

const int PWM_FREQ = 250;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution


DcMotor::DcMotor(OutputPin& dir_pin, PwmPin& pwm_pin){
    __dir_pin = OutputPin(dir_pin);
    __pwm_pin = PwmPin(pwm_pin);
    _motor_type = ACTUATOR_TYPE_T::DC_MOTOR;
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

float DcMotor::read_sensor(){
    return 1234.45;
}

void DcMotor::goto_position(float target_position,float sensor_position){
    float duty = __pid_controller.get_output_value(target_position,sensor_position);
    __pwm_pin.set_duty(duty);
}





