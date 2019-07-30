#ifndef _SERVO_MOTOR_H_
#define _SERVO_MOTOR_H_
// #include "esp32-hal-ledc.h"
// #include "esp32-hal-adc.h"

class ServoMotor{
public:
    void analogWriteESP32(int pin, int value);
    void setup_ledc(int gpio_pin_number);
private:
    void __test();
    int __pin2channel[64]; // holds the PWM channel (0-15) attached to a given pin (0-63)
    void __setupAnalogWritePin(int pin, int channel, int freq=500, int resolution=8);
    int __gpio_pin_number;
};


#endif

