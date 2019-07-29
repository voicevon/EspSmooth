#pragma once

#include <stdint.h>
#include "_hal/Pin.h"

class Pwm: public Pin
{
public:
	Pwm();
	~Pwm(){};
	Pwm(const char* str);
	// bool from_string(const char *str);
	// bool is_valid() const { return valid; }
	// set duty cycle 0-1
	void set(float v);
	float get() const { return value; }
	static uint32_t get_frequency() { return frequency; }
	static bool setup(uint32_t freq);

protected:
	virtual bool check_validation() override;

private:
	bool lookup_pin(uint8_t port, uint8_t pin, uint8_t& ctout, uint8_t& func);
	int map_pin_to_pwm(const char *name);

	static int pwm_index;
	static uint32_t frequency;
	float value;
	bool valid{false};
	uint8_t index;

};
