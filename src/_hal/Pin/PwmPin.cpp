#include "PwmPin.h"

#include <string>
#include <cstring>
#include <cctype>
#include <tuple>
#include <vector>
#include <cmath>

static const int PWM_RESOLUTION_BITS = 16;     // 16 bit resolution

// #include "board.h"

uint32_t PwmPin::frequency= 0;

/* 43xx Pinmap for PWM to CTOUT and function
Pin  a, b, COUT#, Function
*/
// static const std::vector<std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>> lut {
//     {0x01, 1,  7,  1},
//     {0x01, 2,  6,  1},
//     {0x01, 3,  8,  1},
//     {0x01, 4,  9,  1},

// };

// bool PwmPin::lookup_pin(uint8_t port, uint8_t pin, uint8_t& ctout, uint8_t& func)
// {


//     return false;
// }

// static
// int PwmPin::pwm_index= 1;
// int PwmPin::map_pin_to_pwm(const char *name)
// {
//     return 0;
// }



PwmPin::PwmPin(const char *pin)
{
	from_string(pin);
}

// static
bool PwmPin::setup(uint32_t freq)
{

	return true;
}

bool PwmPin::set_frequency_auto_channel(double frequency){
	__pwm_channel = take_pwm_channel();
	ledcSetup(__pwm_channel, frequency, PWM_RESOLUTION_BITS);
    ledcAttachPin(this->get_gpio_num(), __pwm_channel);
	return true;
}


void PwmPin::set(float v)
{
	pwm_value= v;
}

uint8_t PwmPin::take_pwm_channel(){
	return 0;
}
