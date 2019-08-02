#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_
#include "Actuator.h"
#include "_hal/Pin/Pin.h"
#include "_hal/Pin/OutputPin.h"
#include "_hal/Pin/PwmPin.h"

class DcMotor:public Actuator
{
    public:
        // DcMotor(Pin dir, Pin pwm);
        DcMotor(OutputPin* dir_pin, PwmPin* pwm_pin);
        virtual bool step() override;
        virtual void enable(bool state) override;

    private:
        OutputPin* __dir_pin;
        PwmPin* __pwm_pin;
        uint8_t __pwm_channel;
};


#endif
