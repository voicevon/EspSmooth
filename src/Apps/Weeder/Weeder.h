#ifndef _APP_WEEDER_H_
#define _APP_WEEDER_H_

#include "OneSide.h"
class Weeder{
    public:
        static Weeder* get_instance(){ return instance_; }
        OneSide left;
        OneSide right;
        void timer_loop();
        // void time_tick();
    private:
        static Weeder* instance_;
};
#endif