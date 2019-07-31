#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_
#include "Actuator.h"
#include "_hal/Pin.h"

class DcMotor:public Actuator{
public:
    DcMotor(Pin dir, Pin pwm);
    virtual bool step() override;

private:


};


#endif
