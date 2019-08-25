
#include "ServoMotor.h"
#include "HardwareSerial.h" 

const int SERVO_FREQ = 50;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution

const int MIN_PULSE = 1000; // 
const int MAX_PULSE = 65535; // 


ServoMotor::ServoMotor(PwmPin &pwm_pin):Actuator(){
    motor_type_ = ACTUATOR_TYPE_T::SERVO_MOTOR;
    // enabled_ = false;

    __pwm_pin = PwmPin(pwm_pin);   // Is PwmPin() cloning the object from pwm_pin to __pwm_pin ?
}

// Question here:
// Bin size of above: 641133 Bytes
// Bin size of below: 641205 Bytes
// Question: any different with the above and the below ? 
// ServoMotor::ServoMotor(PwmPin pwm_pin):Actuator(){
//     __pwm_pin = pwm_pin;
// }

//virtual override
bool ServoMotor::step() {
    //Calculate and goto target_position
    static float last_angle = 0;
    // Serial.println("a");
    current_position_steps_ += (direction_ ? -1 : 1); 
    return moving_; 
    // return true;
    // float target_angle = get_current_position();
    // return true;

    // if(target_angle != last_angle){
    //     goto_position (target_angle);
    //     last_angle = target_angle;
    // }
    // return true;
}
//virtual override
// void ServoMotor::set_direction(bool new_direction) { 
//     direction= new_direction; 
// }

void ServoMotor::goto_position(float angle) // ,unit = DEGREE)
{
    // float duty =  __map(angle * 100, 0, 36000, MIN_PULSE, MAX_PULSE); 
    float duty =  angle /360 * 65535; 
    __pwm_pin.set_duty(duty);
    // __pwm_pin.set_duty(2000);
}

//virtual override
#define HOME_DUTY 0
void ServoMotor::enable(bool state) {
    // if(enabled_) return;
    
    Serial.println("[D][ServoMotor] enable() at entrance.");
    if(state)
    {
        __pwm_pin.init(SERVO_FREQ, PWM_RESOLUTION_BITS, HOME_DUTY);    // TODO: Home_duty is configable!     
        __pwm_pin.start();
        Serial.println("[D][ServoMotor]: enabled");
        enabled_ = true;
    }else{
        __pwm_pin.stop();
        Serial.println("[D][ServoMotor]: disabled");
        enabled_ = false;
    }
}

float ServoMotor::__map(long x, long in_min, long in_max, long out_min, long out_max){
    long xx= (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return float(xx);
}


