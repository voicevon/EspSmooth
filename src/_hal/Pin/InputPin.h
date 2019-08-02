#pragma once


#include "Pin.h"



class InputPin: public Pin{

    public:
        InputPin(){}   // for moduler
        InputPin(const char* description);
        bool start();
    private:

};