
#include "weeder.h"

void Weeder::timer_loop(){
    left.pid_loop_with_reading_sensors();
    right.pid_loop_with_reading_sensors();
}

