#pragma once
#include "Pin.h"



class OutputPin: public Pin 
{
    public:
        OutputPin(){};
        OutputPin(const char* pin_description, bool start);
        virtual bool start() override;
        virtual bool stop() override;
        bool is_in_range();
        inline void set(bool value) override;

    private:
        bool __is_started;
        static uint8_t __GPIO_RANGE; 
};