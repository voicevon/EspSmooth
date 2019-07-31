#pragma once

// #ifndef _SERVO_MOTOR_H_
// #define _SERVO_MOTOR_H_
#include "_hal/Pin.h"
#include "Actuator.h"

class ServoMotor:public Actuator{
    public:
        ServoMotor();
        ~ServoMotor();
        ServoMotor(int pin_numer);
        void setup(int pin_number);
        void goto_position(float angle);

    private:
        // void __test();
        // Pin out;
        int __gpio_pin_number;  //TODO  Remove this.
        long __map(long x, long in_min, long in_max, long out_min, long out_max);
};


// #endif
