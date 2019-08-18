#include "InputPin.h"

InputPin::InputPin(const char*  description): Pin(description){
	//check range in output {0,1,2,3,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33,34,35,36,39}

}

bool InputPin::start()
{
    if(valid_) {
        if(this->gpio_id_ <= MAX_MCU_GPIO_INDEX ){
            if(this->is_pull_up_) {
                pinMode(this->gpio_id_, INPUT_PULLUP);
                return true;
            } 
            if(this->is_pull_down_){
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
void InputPin::set(bool value){
	digitalWrite(gpio_id_, value ^ inverting_);
}
