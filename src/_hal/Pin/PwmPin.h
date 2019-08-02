#pragma once

#include "Pin.h"
#include <stdint.h>


class PwmPin: public Pin{
	public:
		~PwmPin(){};
		PwmPin(const char* pin);
		uint8_t take_pwm_channel();

		uint32_t get_duty() const { return __duty; }
		void set_duty(uint32_t duty);

		uint32_t get_frequency() { return __frequency; }
		void set_frequency(uint32_t frequency){ __frequency = frequency; }
		
		void set_all(double frequency,uint8_t pwm_width_resolution, uint32_t home_duty );
		bool start();
		void stop();  

	private:
		bool __is_started;

		uint8_t __pwm_channel;
		uint8_t __channel;
		uint32_t __frequency;
		uint32_t __resolution;
		// uint32_t __home_duty;
		float __duty;

};
