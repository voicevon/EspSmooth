#ifndef _ACTUATOR_H_
#define _ACTUATOR_H_
#include "_DRV/Sensor/AdcSensors.h"

class Actuator{
    public:
        void start();
        void attatch_feedback_sensor(Sensor* sensor);
        void attatch_pwm_output(){}
        void attatch_pid_controller(){}
        
        Sensor* __feedback_sensor;
        
};




#endif