#pragma once

#include "Pin.h"
#include <stdint.h>


class PwmPin: public Pin{
	public:
		PwmPin(){};
		~PwmPin(){};
		PwmPin(const char* pin);
		uint8_t take_pwm_channel();

		uint32_t get_duty() const { return __duty; }
		void set_duty(uint32_t duty);

		uint32_t get_frequency() { return __frequency; }
		void set_frequency(uint32_t frequency){ __frequency = frequency; }
		
		void init_all(double frequency,uint8_t pwm_width_resolution, uint32_t duty );
		virtual bool start() override;
		virtual bool stop() override;  
	
		static bool init(); 

	private:
		bool __is_started;
    	static bool set_allocated_channels(uint8_t channel_id, bool set= true);

		uint8_t __pwm_channel;
		uint8_t __channel;
		uint32_t __frequency;
		uint32_t __resolution;
		// uint32_t __home_duty;
		float __duty;

		static uint8_t __channel_index;   


};
