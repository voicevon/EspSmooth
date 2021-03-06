#pragma once

#include "smoothie/smoothie/Module.h"
#include "_hal/Pin/InputPin.h"

class KillButton : public Module {
    public:
        KillButton();
        static bool create(ConfigReader& cr);
        bool configure(ConfigReader& cr);

    private:
        InputPin kill_button;
        void button_tick();

        enum STATE {
            IDLE,
            KILL_BUTTON_DOWN,
            KILLED_BUTTON_DOWN,
            KILLED_BUTTON_UP,
            UNKILL_BUTTON_DOWN,
            UNKILL_TIMING_BUTTON_DOWN,
            UNKILL_FIRE,
            UNKILLED_BUTTON_DOWN
        };

        struct {
            uint8_t unkill_timer:6;
            volatile STATE state:4;
            bool unkill_enable:1;
            bool toggle_enable:1;
        };
};
