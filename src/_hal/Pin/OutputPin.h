#pragma once
#include "Pin.h"

class OutputPin: public Pin 
{
    public:
    
        OutputPin(const char* pin_description);
        bool start();
        bool stop();

    private:

};