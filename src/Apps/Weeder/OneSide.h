#ifndef _APP_WEEDER_ONE_SIDE_H_
#define _APP_WEEDER_ONE_SIDE_H_
#include "_HAL/Pin/PwmPin.h"
#include "_SAL/PID/pid.h"

#include "_DRV/Sensor/AdcSensors.h"

class OneSide{
    public:
        PwmPin actuator_pin;
        
        AdcSensor toucher;
        AdcSensor actuator_feedback;

        OneSide();
        ~OneSide();
        void init_actuator(PwmPin actuator_pin);
        void pid_loop_with_reading_sensors();   
        void set_show_debug(bool config);
        void read_sensors();
        float test_float;
        PID pid_controller; 

    private:
        float __error_length;
        float __error_min = -68.0f;
        float __error_max = 68.0f;
        bool __config_show_debug = false;
        float __get_error_length_of_actuaotr();
        float __convert_command_from_toucher();
        void __show_debug();
        float __target_length;
};

#endif
