#include "InputPin.h"

InputPin::InputPin(const char*  description): Pin(description){

}

bool InputPin::as_input()
{
    if(valid) {
        if(this->gpio_pin_num <= MAX_MCU_GPIO_INDEX ){
            if(this->is_pull_up) {
                pinMode(this->gpio_pin_num, INPUT_PULLUP);
                return true;
            } 
            if(this->is_pull_down){
                pinMode(this->gpio_pin_num, INPUT_PULLDOWN);
                return true;
            }
            pinMode(this->gpio_pin_num, INPUT);
        }else{ //expaned gpio

        }
    }
    return false;
}
