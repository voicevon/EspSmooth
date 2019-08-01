#pragma once

#include "Pin.h"
#include "InputPin.h"

class PinHelper{

    public:
        enum PIN_TYPE_T {AS_INPUT, AS_OUTPUT, AS_PWM, AS_ADC};
        
        PinHelper(){}
        ~PinHelper(){}

        Pin* create_pin(const char* pin_description, PIN_TYPE_T pin_type);
    private:

};

