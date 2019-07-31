#pragma once

#include "_hal/Pin.h"
#include "Actuator.h"

class TMC26X;
class ConfigReader;
class OutputStream;
class GCode;

class StepperMotor: public Actuator
{
    public:
        StepperMotor(Pin& step, Pin& dir, Pin& en);
        ~StepperMotor();
};

