#ifndef _APP_WEEDER_ONE_SIDE_H_
#define _APP_WEEDER_ONE_SIDE_H_
#include "_HAL/Pin/AdcPin.h"
#include "_HAL/Pin/PwmPin.h"


class OneSide{
    public:
        AdcPin toucher_pin;
        AdcPin actuator_feedback_pin;
        PwmPin actuator_pin;
        
        void pid_loop();         
    private:
        // float read_toucher();
        // float read_feedback();
        

};

#endif
