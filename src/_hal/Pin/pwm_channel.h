#ifndef _PWM_CHANNEL_H_
#define _PWM_CHANNEL_H_

#include <stdint.h>

class PwmChannel{
    public:
        // PwmChannel Get_Instance(){return instance};
        
    private:
        PwmChannel(){}
        ~PwmChannel(){}
        static PwmChannel instance;


};

#endif
