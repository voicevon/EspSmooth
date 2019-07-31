#include "StepperMotor.h"
#include "robot/StepTicker.h"

#include <math.h>

StepperMotor::StepperMotor(Pin &step, Pin &dir, Pin &en) : Actuator( step, dir, en){

}
StepperMotor::~StepperMotor(){

}
