#ifndef _ACTUATOR_TYPE_H_
#define _ACTUATOR_TYPE_H_

#include "string"
#include "stdint.h"

class ActuatorType{
    
    public:
        ActuatorType(std::string str);
        ~ActuatorType(){}

        ActuatorType* from_string(std::string str);

        uint8_t toInt();

    private:
        uint8_t id;
};

#endif
