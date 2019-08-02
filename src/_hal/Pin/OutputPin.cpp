#include "OutputPin.h"

OutputPin::OutputPin(const char* pin_description):Pin(pin_description){

}

bool OutputPin::init()
{
    if(this->__valid) {
        if(this->__gpio_id <= MAX_MCU_GPIO_INDEX){
            if(this->open_drain){                                   //need to check?
                pinMode(this->__gpio_id, OUTPUT_OPEN_DRAIN);   
                return true;
            }
            pinMode(this->__gpio_id, OUTPUT);
        }else{  //expaned io
            
        }
    }
    return false;
}

