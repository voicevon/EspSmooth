#include "PwmPin.h"
// #include "esp32-hal-ledc.h"    //Why it still work?

#include <string>
#include <cstring>
#include <cctype>
#include <tuple>
#include <vector>
#include <cmath>

// static const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution


// uint32_t PwmPin::frequency= 0;




PwmPin::PwmPin(const char *pin)
{
	from_string(pin);
	__is_started = false;
}

// // static
// bool PwmPin::setup(uint32_t freq)
// {

// 	return true;
// }

void PwmPin::set_all(double frequency,uint8_t resolution,uint32_t duty ){
	__pwm_channel = take_pwm_channel();
	__frequency = frequency;
	__resolution = resolution;
	__duty = duty;
}

bool PwmPin::start(){
	ledcSetup(__pwm_channel, __frequency, __resolution );
    ledcAttachPin(this->get_gpio_num(), __pwm_channel);
	ledcWrite(this->get_gpio_num(),__duty);
	__is_started = true;
	return true;
}

 //set gpio to input mode. ??
void PwmPin::stop(){
	__is_started =  false;   //rename to "__is_working" ?
}

void PwmPin::set_duty(uint32_t duty) { 
	__duty = duty;
	if(__is_started){
		ledcWrite(this->get_gpio_num(),duty);
	}
}


uint8_t PwmPin::take_pwm_channel(){
	return 0;
}


// bitset to indicate a pin has been configured
#include <bitset>
static std::bitset<GPIO_PINS_COUNT> allocated_channels;   //default constructor :The object is initialized with zeros.
bool PwmPin::set_allocated_channels(uint8_t channel_id, bool set)
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