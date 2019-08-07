#pragma once

//declare extern as global.  it's outside of esphome
#include "esphome/components/logger/logger.h"
extern esphome::logger::Logger *logger_logger;


 int16_t current_working_state;
 void automation_setup(){};
 void automation_loop(){};

