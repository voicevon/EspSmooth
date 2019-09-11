#ifndef _APP_WEEDER_H_
#define _APP_WEEDER_H_

#include "_HAL/Pin/OutputPin.h"
#include "_HAL/Pin/InputPin.h"
#include "OneSide.h"
class Weeder{
    public:
        static Weeder* get_instance(){ return __instance; }
        OneSide left;
        OneSide right;
        void timer_loop();
        void init();
    private:
        Weeder();
        static Weeder* __instance;
        OutputPin __k1_pin;
        OutputPin __k2_pin;
        OutputPin __k3_pin;
        InputPin __button_a;
        InputPin __button_b;

};
#endif