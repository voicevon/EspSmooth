#include "InputPin.h"

InputPin::InputPin(const char*  description): Pin(description){

}

bool InputPin::start()
{
    if(__valid) {
        if(this->__gpio_id <= MAX_MCU_GPIO_INDEX ){
            if(this->is_pull_up) {
                pinMode(this->__gpio_id, INPUT_PULLUP);
                return true;
            } 
            if(this->is_pull_down){
                pinMode(this->__gpio_id, INPUT_PULLDOWN);
                return true;
            }
            pinMode(this->__gpio_id, INPUT);
        }else{ //expaned gpio

        }
    }
    return false;
}
