#include "DcMotor.h"
#include "_hal/Pin/PinHelper.h"

const int PWM_FREQ = 250;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution
const int HOME_DUTY = 0;

DcMotor::DcMotor(OutputPin& dir_pin, PwmPin& pwm_pin){
    motor_type_ = ACTUATOR_TYPE_T::DC_MOTOR;
    __enabled = false;

    __dir_pin = OutputPin(dir_pin);
    __pwm_pin = PwmPin(pwm_pin);

}

bool DcMotor::step(){
    // pwm_pin.set_value(this->get_current_position());
    return true;
}

//virtual  override
void DcMotor::enable(bool state){
    if(__enabled) return;
    
    Serial.println("[D][DcMotor] enable() at entrance.");
    if(state)
    {
        __dir_pin.start();
        __pwm_pin.init(PWM_FREQ, PWM_RESOLUTION_BITS, HOME_DUTY);    // TODO: Home_duty is configable!     
        __pwm_pin.start();
        __enabled = true;

        Serial.print("[D][DcMotor]: enabled pwm_channel= ");
        Serial.print(__pwm_pin.get_channel());
        Serial.println("");
    }else{
        __dir_pin.stop();
        __pwm_pin.stop();
        __enabled = false;
        Serial.println("[D][DcMotor]: disabled");
    }
}

// float DcMotor::read_sensor_position(){
//     __sensor_position = __ads1115.sample();
//     return __sensor_position;
// }

void DcMotor::goto_position(float target_position){
    // float duty = __pid_controller.get_output_value(target_position,__sensor_position);
    __pwm_pin.set_duty(123);
}









