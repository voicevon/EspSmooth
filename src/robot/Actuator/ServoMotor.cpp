
#include "ServoMotor.h"

const int SERVO_FREQ = 250;    // 50Hz  20ms 
const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution

const int MIN_PULSE = 1000; // 
const int MAX_PULSE = 75000; //

// static uint8_t pwm_channel = 0;


ServoMotor::ServoMotor(PwmPin& pwm_pin):Actuator(){
    __pwm_pin = PwmPin(pwm_pin);
}


bool ServoMotor::step() {
    //Calculate and goto target_position
    float target_angle = get_current_position();
    goto_position (target_angle);
}

// #include "HardwareSerial.h"
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
void ServoMotor::enable(bool state) {
    if(state)
    {
        __pwm_pin.start();
    }else{
        __pwm_pin.stop();
    }
}

float ServoMotor::__map(long x, long in_min, long in_max, long out_min, long out_max){
    long xx= (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return float(xx);
}


