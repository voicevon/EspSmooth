#pragma once

#include "InputPin.h"
#include "OutputPin.h"
#include "PwmPin.h"
#include "AdcPin.h"

class PinHelper{

    public:
        
        PinHelper(){}
        ~PinHelper(){}

        enum PIN_TYPE_T {AS_INPUT, AS_OUTPUT, AS_PWM, AS_ADC};
        Pin* create_pin(const char* pin_description, PIN_TYPE_T pin_type);
        
        PwmPin* Create_PwmPin(const char* pin_description);

    private:

};

