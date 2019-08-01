#include "ActuatorType.h"
#include "HardwareSerial.h"
#include "libs/StringUtils.h"

ActuatorType::ActuatorType(const char* type_description){
    from_string(type_description);
}



 ActuatorType* ActuatorType::from_string(const char* type_description){
    std::string upper = stringutils::toUpper(type_description);
    if(upper == "STEPPER") { id = STEPPER_MOTOR; } 
    else if(upper == "SERVO") { id = SERVO_MOTOR; }
    else if(upper == "XUEFENG") { id = XUEFENG_MOTOR; }
    else if(upper == "DC") { id = DC_MOTOR; }
    else {
        Serial.print("[E][ActuatorType][from_string()] type_description= >");
        Serial.print(type_description);
        Serial.println("<    Forced to set as STEPPER_MOTOR");
        id = STEPPER_MOTOR;
    }
    return this;  
}

// // 0=stepper  1=servo  2=xuefeng  3=dc 
// uint8_t ActuatorType::toInt(){
//     return this->id;
// }
