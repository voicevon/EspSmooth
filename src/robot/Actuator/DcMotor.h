#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_
#include "Actuator.h"
#include "_hal/Pin/Pin.h"
#include "_hal/Pin/OutputPin.h"
#include "_hal/Pin/PwmPin.h"
#include "libs/MotorPidController.h"
#include "component/ads1115.h"

class DcMotor:public Actuator
{
    public:
        
        DcMotor(OutputPin& dir_pin, PwmPin& pwm_pin);
        virtual bool step() override;
        virtual void enable(bool state) override;
        void goto_position(float target_position);
        // float read_sensor_position();

    private:
        OutputPin __dir_pin;
        PwmPin __pwm_pin;
        uint8_t __pwm_channel;
        bool __enabled;
        // float __sensor_position;
        // MotorPidController __pid_controller;
        // esphome::ads1115::ADS1115Component* ads1115_component;
        // esphome::ads1115::ADS1115Sensor __ads1115;
};


#endif
