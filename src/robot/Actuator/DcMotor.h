#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_
#include "Actuator.h"
#include "_hal/Pin/Pin.h"

class DcMotor:public Actuator{

public:
     DcMotor(Pin dir, Pin pwm);
    // DcMotor(OutputPin dir_pin, PwmPin pwm_pin);
    virtual bool step() override;

private:
    Pin __pin_dir;
    Pin __pin_pwm;
    uint8_t __pwm_channel;
};


#endif
