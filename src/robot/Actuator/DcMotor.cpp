#include "DcMotor.h"
#include "_hal/Pin/PinHelper.h"
#include "stdio.h"
#include "HardwareSerial.h"

const int PWM_FREQ = 50;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution
const int HOME_DUTY = 0;

DcMotor::DcMotor(OutputPin& dir_pin, PwmPin& pwm_pin,esphome::ads1115::ADS1115Sensor& ads1115_sensor){
    motor_type_ = ACTUATOR_TYPE_T::DC_MOTOR;
    __enabled = false;

    __dir_pin = OutputPin(dir_pin);
    __pwm_pin = PwmPin(pwm_pin);

    //  __ads1115 = ads1115_sensor; 
}

// Called by timer.
void DcMotor::pid_loop(float target_position){
    if(!__enabled) return;   //??
    Serial.print("*");
    // __sensor_position = __ads1115.sample();
    if(__sensor_position <10) return;    // there is an error.

    if(isnan(__sensor_position)) return;  //??
    float duty = __pid_controller.get_output_value(target_position,__sensor_position);
    duty *= 10.0f;
    // printf("state =%i, target= %f, sensor = %f, duty= %f \n", __enabled, target_position, __sensor_position,duty);
    __pwm_pin.set_duty(duty);
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

float DcMotor::for_test_read_sensor_position(){
    // __sensor_position = __ads1115.sample();
    return __sensor_position;
}

void DcMotor::for_test_goto_position(float target_position){
    __pwm_pin.set_duty(target_position);
}









