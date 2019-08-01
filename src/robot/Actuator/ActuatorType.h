#ifndef _ACTUATOR_TYPE_H_
#define _ACTUATOR_TYPE_H_

#include "string"
#include "stdint.h"

class ActuatorType{

    public:

        enum ACTUATOR_TYPE_T {STEPPER_MOTOR, SERVO_MOTOR, XUEFENG_MOTOR, DC_MOTOR};

        ActuatorType(const char* type_description);
        ~ActuatorType(){}

        ActuatorType* from_string(const char*  type_description);

        ACTUATOR_TYPE_T get_type() { return id; }

    private:
        ACTUATOR_TYPE_T id;

};

#endif
