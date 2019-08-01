#ifndef _ACTUATOR_TYPE_H_
#define _ACTUATOR_TYPE_H_

#include "string"
#include "stdint.h"

class ActuatorType{

    enum ACTUATOR_TYPE_T {STEPPER_MOTOR,SERVO_MOTOR,XUEFENG_MOTOR,DC_MOTOR};

    public:
        ActuatorType(const char* type_description);
        ~ActuatorType(){}

        ActuatorType* from_string(const char*  type_description);

        uint8_t toInt();

    private:
        uint8_t id;
};

#endif
