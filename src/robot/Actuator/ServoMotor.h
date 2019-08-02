#pragma once

// #ifndef _SERVO_MOTOR_H_
// #define _SERVO_MOTOR_H_
#include "_hal/Pin/PwmPin.h"
#include "Actuator.h"

class ServoMotor:public Actuator{
    public:
        ServoMotor(PwmPin* pwm_pin);
        void goto_position(float angle);
        virtual bool step() override;

        virtual void enable(bool state) override;

    private:
        PwmPin* __pwm_pin;
        uint8_t __pwm_channel;
        float __map(long x, long in_min, long in_max, long out_min, long out_max);
};


// #endif
