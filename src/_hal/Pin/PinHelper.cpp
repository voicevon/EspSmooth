#include "PinHelper.h"


// Pin* PinHelper::create_pin(const char* pin_description, PinHelper::PIN_TYPE_T pin_type){
//     switch (pin_type)
//     {
//     case 1:
//         /* code */
//         break;
    
//     default:
//         break;
//     }
//     auto xx= new Pin();
//     return xx;
// }

PwmPin* Create_PwmPin(const char* pin_description){
    PwmPin* xx = new PwmPin(pin_description);
    PwmPin pp(pin_description);
    return &pp;
    // return xx;
}

