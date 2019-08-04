#pragma once

#include "Pin.h"
#include <stdint.h>
#include "HardwareSerial.h"


class PwmPin: public Pin{
	public:
		PwmPin(){};
		~PwmPin(){};
		PwmPin(const char* pin);

		uint32_t get_duty() const { return __duty; }
		void set_duty(uint32_t duty);
		virtual void set(bool) {Serial.println( " [E][PwmPin] Don't invoke me!!!"); }

		uint32_t get_frequency() { return __frequency; }
		void set_frequency(uint32_t frequency){ __frequency = frequency; }
		
		void init(double frequency,uint8_t pwm_width_resolution, uint32_t duty );
		virtual bool start() override;
		virtual bool stop() override;  
	
		static bool init_channel_flags(); 
		uint8_t get_channel(){ return __channel; } 

	private:
		bool __is_started;
    	static bool __set_allocated_channels(uint8_t channel_id, bool set);
		uint8_t __take_pwm_channel();

		uint8_t __channel;
		uint32_t __frequency;
		uint32_t __resolution;
		uint32_t __duty;

		static uint8_t __channel_index_wizard;   


};
