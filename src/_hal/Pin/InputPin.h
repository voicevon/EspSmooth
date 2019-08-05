#pragma once


#include "Pin.h"



class InputPin: public Pin{

    public:
        InputPin(){}   // for moduler
        InputPin(const char* description);
        virtual bool start() override;
        virtual bool stop() override;
        
    private:
        static uint8_t __GPIO_RANGE; 

};