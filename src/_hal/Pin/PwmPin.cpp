#include "PwmPin.h"
// #include "esp32-hal-ledc.h"    //Why it still work?
#include "HardwareSerial.h"

#include <string>
#include <cstring>
#include <cctype>
#include <tuple>
#include <vector>
#include <cmath>



PwmPin::PwmPin(const char *pin)
{
	from_string(pin);
	__is_started = false;
	__pwm_channel = 255;
}

void PwmPin::init(double frequency,uint8_t resolution,uint32_t duty ){
	__pwm_channel = __take_pwm_channel();
	if(__pwm_channel != 255){
		__frequency = frequency;
		__resolution = resolution;
		__duty = duty;
	}
}

bool PwmPin::start(){
	if(__pwm_channel == 255){
		ledcSetup(__pwm_channel, __frequency, __resolution );
		ledcAttachPin(this->get_gpio_id(), __pwm_channel);
		ledcWrite(this->get_gpio_id(),__duty);
		__is_started = true;
		Serial.print("[D][PwmPin] start pwm_channel= ");
		Serial.print(__pwm_channel);
		Serial.print(",   Frequency= ");
		Serial.print(__frequency);
		Serial.print(",  PWM_resolution bits= ");
		Serial.print(__resolution);
		Serial.print( ",  output_pin= ");
		Serial.println(this->get_gpio_id());
		return true;
	}
	Serial.println("[E][PwmPin][start()] Have NOT got a channel");
	return false;
}

 //set gpio to input mode. ??
bool PwmPin::stop(){
	__is_started =  false;   //rename to "__is_working" ?
	return false;
}

void PwmPin::set_duty(uint32_t duty) { 
	__duty = duty;
	if(__is_started){
		ledcWrite(this->get_gpio_id(),duty);
	} else {
		start();
		ledcWrite(this->get_gpio_id(),duty);
	}

}

// static
uint8_t PwmPin::__take_pwm_channel(){
	bool sucessed = __set_allocated_channels(__channel_index);
	if (sucessed){
		__channel_index++;
		return __channel_index - 1;
	}
	Serial.println("[E][PwmPin][take_pwm_channel] the channel is allocated previous.");
	return 255L;
}

// static
uint8_t PwmPin::__channel_index = 0;
bool PwmPin::init_channel_flags() { 
	__channel_index = 0;
	return true;
}

// bitset to indicate a pin has been configured
#include <bitset>
static std::bitset<GPIO_PINS_COUNT> allocated_channels;   //default constructor :The object is initialized with zeros.
bool PwmPin::__set_allocated_channels(uint8_t channel_id, bool set)
{
    if(!set) {
        // deallocate it
        allocated_channels.reset(channel_id);
        return true;
    }

    if(!allocated_channels[channel_id]) {
        // if not set yet then set it
        allocated_channels.set(channel_id);
        return true;
    }

    // indicate it was already set
    return false;
}