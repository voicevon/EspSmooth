# include "espsmooth.h"

using namespace espsmooth;

endstop::EndstopSensor * endstop_x;
endstop::EndstopSensor * temperature_hotend;

void setup(){
    endstop_x = new endstop::EndstopSensor();
    App.register_component(endstop_x);

    temperature_hotend = new endstop::EndstopSensor();
    App.register_component(temperature_hotend);

    App.setup();    
}

void loop(){
    App.loop();
}