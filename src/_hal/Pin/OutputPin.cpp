#include "OutputPin.h"

OutputPin::OutputPin(const char* pin_description):Pin(pin_description){

}

bool OutputPin::init()
{
    if(this->valid) {
        if(this->gpio_pin_num <= MAX_MCU_GPIO_INDEX){
            if(this->open_drain){                                   //need to check?
                pinMode(this->gpio_pin_num, OUTPUT_OPEN_DRAIN);   
                return true;
            }
            pinMode(this->gpio_pin_num, OUTPUT);
        }else{  //expaned io
            
        }
    }
    return false;
}

