
#include "esp32-hal-ledc.h"
#include "ServoMotor.h"

const int SERVO_CHANNEL = 0;  // 0-15
const int SERVO_FREQ = 250;    // 50Hz  20ms 
const int SERVO_RES = 16;     // 16 bit resolution

const int MIN_PULSE = 1000; // 
const int MAX_PULSE = 75000; //

ServoMotor::ServoMotor(){

}
ServoMotor::~ServoMotor(){

}
ServoMotor::ServoMotor(int pin_number):Actuator(){
    setup(pin_number);
}

void ServoMotor::setup(int pin_number){
    __gpio_pin_number = pin_number;
    ledcSetup(SERVO_CHANNEL, SERVO_FREQ, SERVO_RES);
    ledcAttachPin(__gpio_pin_number,SERVO_CHANNEL);
    ledcWrite(SERVO_CHANNEL, 0);
}

bool ServoMotor::step() {
    //Calculate and goto target_position
    float target_angle = get_current_position();
    goto_position (target_angle);
}

// #include "HardwareSerial.h"
void ServoMotor::goto_position(float angle) // ,unit = DEGREE)
{
    int pwm = __map(angle * 100, 0, 36000, MIN_PULSE, MAX_PULSE); 
    ledcWrite(SERVO_CHANNEL, pwm);
    // Serial.print ("    ");
    // Serial.print(val);
    // Serial.print ("    ");
}

long ServoMotor::__map(long x, long in_min, long in_max, long out_min, long out_max){
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }


