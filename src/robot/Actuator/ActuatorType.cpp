#include "ActuatorType.h"

ActuatorType::ActuatorType(const char* type_description){
    from_string(type_description);
}

ActuatorType* ActuatorType::from_string(const char* type_description){
    if(type_description == "stepper") this->id = 1;
    if(type_description == "servo") this->id = 2;
    if(type_description == "xuefeng") this->id = 3;
    if(type_description == "dc") this->id = 4;
    return this;  
}

uint8_t ActuatorType::toInt(){
    return this->id;
}
