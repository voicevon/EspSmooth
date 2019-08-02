#pragma once
#include "Pin.h"

class OutputPin: public Pin 
{
    public:
        OutputPin(){};
        OutputPin(const char* pin_description);
        virtual bool start() override;
        virtual bool stop() override;

    private:

};