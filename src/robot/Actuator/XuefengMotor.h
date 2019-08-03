#pragma once
#include "Actuator.h"

class XuefengMotor:public Actuator{
    public:

        XuefengMotor();
        void goto_position(float);
    private:

    
};
