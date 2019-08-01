#pragma once

#include "Pin.h"
#include <stdint.h>


class PwmPin: public Pin
{
public:
	PwmPin();
	~PwmPin(){};
	PwmPin(const char* pin);
	void set(float v);
	float get_pwm() const { return pwm_value; }
	static uint32_t get_frequency() { return frequency; }
	static bool setup(uint32_t freq);

private:
	bool lookup_pin(uint8_t port, uint8_t pin, uint8_t& ctout, uint8_t& func);
	int map_pin_to_pwm(const char *name);

	static int pwm_index;
	static uint32_t frequency;
	float pwm_value;
	uint8_t index;

};
