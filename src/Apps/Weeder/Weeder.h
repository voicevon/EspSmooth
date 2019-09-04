#ifndef _APP_WEEDER_H_
#define _APP_WEEDER_H_

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
};
#endif