#pragma once
#include "Actuator.h"

class XuefengMotor:public Actuator{
    public:

        XuefengMotor();
        void goto_position(float);
        void set_direction(bool new_direction) override{};

    private:

    
};
