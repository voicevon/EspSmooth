
#include "ServoMotor.h"
#include "HardwareSerial.h" 

const int SERVO_FREQ = 250;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution

const int MIN_PULSE = 1000; // 
const int MAX_PULSE = 75000; //


ServoMotor::ServoMotor(PwmPin &pwm_pin):Actuator(){
    __pwm_pin = PwmPin(pwm_pin);   // Is PwmPin() cloning the object from pwm_pin to __pwm_pin ?

}

// Question here:
// Bin size of above: 641133 Bytes
// Bin size of below: 641205 Bytes
// Question: any different with the above and the below ? 
// ServoMotor::ServoMotor(PwmPin pwm_pin):Actuator(){
//     __pwm_pin = pwm_pin;
// }


bool ServoMotor::step() {
    //Calculate and goto target_position
    float target_angle = get_current_position();
    // Serial.println(target_angle);
    goto_position (target_angle);
}

void ServoMotor::goto_position(float angle) // ,unit = DEGREE)
{
    float duty =  __map(angle * 100, 0, 36000, MIN_PULSE, MAX_PULSE); 
    __pwm_pin.set(duty);
    // ledcWrite(pwm_channel, pwm);
    // Serial.print ("    ");
    // Serial.print(val);
    // Serial.print ("    ");
}

//virtual override
#define HOME_DUTY 0
void ServoMotor::enable(bool state) {
    if(state)
    {
        __pwm_pin.init_all(SERVO_FREQ,PWM_RESOLUTION_BITS,HOME_DUTY);    // TODO: Home_duty is configable!     
        __pwm_pin.start();
        Serial.println("[D][ServoMotor]: enabled");
    }else{
        __pwm_pin.stop();
        Serial.println("[D][ServoMotor]: disabled");
    }
}

float ServoMotor::__map(long x, long in_min, long in_max, long out_min, long out_max){
    long xx= (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return float(xx);
}


