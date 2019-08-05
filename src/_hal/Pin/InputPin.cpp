#include "InputPin.h"

InputPin::InputPin(const char*  description): Pin(description){

}

bool InputPin::start()
{
    if(valid_) {
        if(this->gpio_id_ <= MAX_MCU_GPIO_INDEX ){
            if(this->is_pull_up) {
                pinMode(this->gpio_id_, INPUT_PULLUP);
                return true;
            } 
            if(this->is_pull_down){
                pinMode(this->gpio_id_, INPUT_PULLDOWN);
                return true;
            }
            pinMode(this->gpio_id_, INPUT);
        }else{ //expaned gpio

        }
    }
    return false;
}

// to do what? diable the pull-up, pull-down? 
bool InputPin::stop(){
    return false;
}
