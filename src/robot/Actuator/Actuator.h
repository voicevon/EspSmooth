#ifndef _ACTUATOR_H_
#define _ACTUATOR_H_


#include "_hal/Pin.h"


class Actuator
{
    public:
        Actuator();
        ~Actuator();

        void set_motor_id(uint8_t id) { motor_id = id; }
        uint8_t get_motor_id() const { return motor_id; }
        
     
    protected:



        volatile struct {
            uint8_t motor_id:8;
            volatile bool direction:1;
            volatile bool moving:1;
            bool selected:1;
            bool extruder:1;
        };
    private:


};



#endif
