#pragma once

#include "smoothie/robot/Robot.h"

class AutoPushPop
{
public:
    AutoPushPop(){ Robot::getInstance()->push_state(); }
    ~AutoPushPop(){ Robot::getInstance()->pop_state(); }
};
