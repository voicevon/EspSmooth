#ifndef _PWM_ACTUATOR_H_
#define _PWM_ACTUATOR_H_
#include "actuator.h"
#include "_HAL/Pin/PwmPin.h"

class PwmActuator: public Actuator{
    public:
        PwmActuator(PwmPin pwm_pin,int frequency, int resolution,int default_value);
        float get_error();
    private:
        PwmPin __pwm_pin;
};


#endif