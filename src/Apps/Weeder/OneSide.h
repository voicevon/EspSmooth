#ifndef _APP_WEEDER_ONE_SIDE_H_
#define _APP_WEEDER_ONE_SIDE_H_
#include "_HAL/Pin/AdcPin.h"
#include "_HAL/Pin/PwmPin.h"
#include "_SAL/PID/pid.h"


class OneSide{
    public:
        AdcPin toucher_pin;
        AdcPin actuator_feedback_pin;
        PwmPin actuator_pin;
        OneSide();
        ~OneSide();
        void init(AdcPin toucher_pin,AdcPin feedback_pin, PwmPin actuator_pin,float pid_p,float pid_i,float pid_d);
        void init();
        void pid_loop_with_reading_sensors();   
        void read_sensors();
        float get_toucher_length(){ return __toucher_length; }
        float get_feedback_length(){ return __feedback_length;}
        float test_float;
    private:
        float __toucher_length;
        float __feedback_length;
        float __toucher_to_length(float adc_value);
        float __feedback_to_length(float adc_value);
        float __error_length;
        PID __pid_controller; 
        float __get_error_length_of_actuaotr();
        float __convert_command_from_toucher();
        float __toucher_slope;
        float __toucher_offset;
        float __feedback_slope;
        float __feedback_offset;

};

#endif
