#ifndef _ACTUATOR_H_
#define _ACTUATOR_H_


#include "stdint.h"
#include "esp_attr.h"

class Actuator
{
    public:
        //TMC2208 @UART,   TMC26XX @SPI
        enum ACTUATOR_TYPE_T {STEPPER_MOTOR, TMC22xx_MOTOR, TMC26XX_MOTOR,SERVO_MOTOR, XUEFENG_MOTOR, DC_MOTOR};

        Actuator();
        ~Actuator();
        static ACTUATOR_TYPE_T get_type_from_string(const char* type_description);
        void set_motor_id(uint8_t id) { motor_id_ = id; }
        uint8_t get_motor_id() const { return motor_id_; }
        
        // called from step ticker ISR
        virtual bool step() = 0;
        // called from unstep ISR
        virtual void unstep() {}
        // called from step ticker ISR
        virtual void set_direction(bool new_direction) = 0;

        // Does a manual step pulse, used for direct encoder control of a stepper
        // NOTE manual step is experimental and may change and/or be removed in the future, it is an unsupported feature.
        // use at your own risk
        virtual void manual_step(bool dir) {}

        virtual void enable(bool state) = 0;
        bool is_enabled() const { return enabled_; }
        inline bool is_moving() const { return moving_; };
        IRAM_ATTR void start_moving() { moving_= true; }
        inline  void stop_moving() { moving_= false; }


        bool which_direction() const { return direction_; }

        float get_steps_per_second()  const { return steps_per_second_; }
        float get_steps_per_mm()  const { return steps_per_mm_; }
        void change_steps_per_mm(float);
        void change_last_milestone(float);
        void set_last_milestones(float, int32_t);
        void update_last_milestones(float mm, int32_t steps);
        float get_last_milestone(void) const { return last_milestone_mm_; }
        int32_t get_last_milestone_steps(void) const { return last_milestone_steps_; }
        float get_current_position(void) const { return (float)current_position_steps_/steps_per_mm_; }
        uint32_t get_current_step(void) const { return current_position_steps_; }
        float get_max_rate(void) const { return max_rate_; }
        void set_max_rate(float mr) { max_rate_= mr; }
        void set_acceleration(float a) { acceleration_= a; }
        float get_acceleration() const { return acceleration_; }
        bool is_selected() const { return selected; }
        void set_selected(bool b) { selected= b; }
        bool is_extruder() const { return extruder; }
        void set_extruder(bool b) { extruder= b; }

        int32_t steps_to_target(float);

        // TYPE_T type_from_string(const char* type_description);
        ACTUATOR_TYPE_T get_motor_type() const { return motor_type_; }

    protected:
        void _init();
        float steps_per_second_;
        float steps_per_mm_;
        float max_rate_; // this is not really rate it is in mm/sec, misnamed used in Robot and Extruder
        float acceleration_;

        volatile int32_t current_position_steps_;
        int32_t last_milestone_steps_;
        float   last_milestone_mm_;

        volatile struct {
            uint8_t motor_id_:8;
            volatile bool direction_:1;
            volatile bool moving_:1;
            bool selected:1;
            bool extruder:1;
        };
        ACTUATOR_TYPE_T motor_type_;
        bool enabled_ = false;  

    private:


};



#endif
