#include "OutputPin.h"
#include "HardwareSerial.h"

static const uint8_t __GPIO_RANGE[20]= {0,2,4,5,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};
// static const uint8_t __GPIO_RANGE[20];
OutputPin::OutputPin(const char* pin_description,bool auto_start){
	from_string(pin_description);
	__is_started = false;   // to be a procted variable?
	valid_ &= is_in_range();
	if(!valid_) return;
	if(auto_start){
		this->start();
		__is_started = true;
	}
	return;
	// TODO:
	//check if the input pin_number is in the avaliable range.
	// for(int i=0; i<20; i++){
	// 	if(__GPIO_RANGE[i] == get_gpio_id()){
	// 		return;
	// 	}
	// }
	printf("[E][OutputPin] GPIO_ i% is NOT suitable for output.\n");
}
bool OutputPin::is_in_range(){
	return true;
}
void OutputPin::set(bool value){
	digitalWrite(gpio_id_, value ^ inverting_);
}
bool OutputPin::start()
{
	if(__is_started) return true;
	if(is_pull_up_)
		digitalWrite(gpio_id_,HIGH);
	if(is_pull_down_)
		digitalWrite(gpio_id_,LOW);

    if(this->open_drain){                                   //need to check?
        pinMode(this->gpio_id_, OUTPUT_OPEN_DRAIN);   
    }else{
        pinMode(this->gpio_id_, OUTPUT);
    }

	__is_started = true;
	// Serial.print("[D][OutputPin][start()]   output_pin= GPIO_= ");
	// Serial.println(this->get_gpio_id());
	// Serial.print( ", Open_drain= ");
	// Serial.print(this->open_drain);
	printf("[D][OutputPin] start() GPIO= %i,open_drain= %d\n", gpio_id_, open_drain);
	return true;
}

//Set as inpput?
bool OutputPin::stop(){
	__is_started =  false;   //rename to "__is_working" ?
	Serial.print("[D][OutputPin] stop() output_pin= GPIO_");
	Serial.println(this->get_gpio_id());
	return true;
}

