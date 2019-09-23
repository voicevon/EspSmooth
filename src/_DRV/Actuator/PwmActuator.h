#ifndef _PWM_ACTUATOR_H_
#define _PWM_ACTUATOR_H_
#include "actuator.h"
#include "_HAL/Pin/PwmPin.h"
#include "_DRV/Sensor/AdcSensors.h"

class PwmActuator: public Actuator{
    public:
        PwmActuator(PwmPin pwm_pin,int frequency, int resolution,int default_value);
        void attatch_feedback_sensor(Sensor* sensor);
        void start();
        float get_error();
    private:
        PwmPin __pwm_pin;
        Sensor* __feedback_sensor;
};


#endif