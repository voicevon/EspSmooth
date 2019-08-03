#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_
#include "Actuator.h"
#include "_hal/Pin/Pin.h"
#include "_hal/Pin/OutputPin.h"
#include "_hal/Pin/PwmPin.h"
#include "libs/MotorPidController.h"
#include "robot/drivers/Ads1115.h"

class DcMotor:public Actuator
{
    public:
        
        DcMotor(OutputPin& dir_pin, PwmPin& pwm_pin);
        virtual bool step() override;
        virtual void enable(bool state) override;
        void goto_position(float target_position);

    private:
        OutputPin __dir_pin;
        PwmPin __pwm_pin;
        uint8_t __pwm_channel;
        MotorPidController __pid_controller;
        Ads1115 __ads1115;
        float __read_sensor();
        void __goto_position(float target_position,float sensor_position);
};


#endif
