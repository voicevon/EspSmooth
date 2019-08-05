#include "OutputPin.h"
#include "HardwareSerial.h"

OutputPin::OutputPin(const char* pin_description){
	from_string(pin_description);
	__is_started = false;   // to be a procted variable?
}

bool OutputPin::start()
{
	if(__is_started) return true;

    if(this->open_drain){                                   //need to check?
        pinMode(this->gpio_id_, OUTPUT_OPEN_DRAIN);   
    }else{
        pinMode(this->gpio_id_, OUTPUT);
    }

	__is_started = true;
	Serial.print("[D][OutputPin] start Open_drain= ");
	Serial.print(this->open_drain);
	Serial.print( ",  output_pin= GPIO_");
	Serial.println(this->get_gpio_id());
	return true;
}

//Set as inpput?
bool OutputPin::stop(){
	__is_started =  false;   //rename to "__is_working" ?
	Serial.print("[D][OutputPin] stop() output_pin= GPIO_");
	Serial.println(this->get_gpio_id());
	return true;
}

