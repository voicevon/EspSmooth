#pragma once
#include "Pin.h"

class OutputPin: public Pin 
{
    public:
    
        OutputPin(const char* pin_description);
        virtual bool init() override;

    private:

};