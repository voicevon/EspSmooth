#pragma once

#include "_hal/Pin.h"
#include "Actuator.h"

class TMC26X;
class ConfigReader;
class OutputStream;

class StepperMotor:public Actuator
{
    public:
        StepperMotor(Pin& step, Pin& dir, Pin& en);
        ~StepperMotor();


        inline virtual bool step() override;
        inline virtual void unstep() override;
        inline virtual void set_direction(bool f) override;
        virtual void manual_step(bool dir) override;
        virtual void enable(bool state) override;
        virtual bool is_enabled() const override;


    private:
        Pin step_pin;
        Pin dir_pin;
        Pin en_pin;
      


#ifdef BOARD_PRIMEALPHA
    public:
        bool set_current(float c);
        bool set_microsteps(uint16_t ms);
        int get_microsteps();
        bool setup_tmc2660(ConfigReader& cr, const char *actuator_name);
        bool init_tmc2660();
        void dump_status(OutputStream& os, bool flag=true);
        void set_raw_register(OutputStream& os, uint32_t reg, uint32_t val);
        bool set_options(GCode& gcode);
        bool check_driver_error();
        static bool set_vmot(bool state) { bool last= vmot; vmot= state; return last; }
        void set_vmot_lost() { vmot_lost= true; }

    private:
        // TMC2660 driver
        TMC26X *tmc2660{nullptr};
        static bool vmot;
        bool vmot_lost{true};
#endif
};

