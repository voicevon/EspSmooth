#include "ActuatorType.h"

ActuatorType::ActuatorType(std::string str){
    from_string(str);
}

ActuatorType* ActuatorType::from_string(std::string str){
    if(str == "stepper") this->id = 1;
    if(str == "servo") this->id = 2;
    if(str == "dc") this->id = 3;
    return this;  
}

uint8_t ActuatorType::toInt(){
    return this->id;
}
