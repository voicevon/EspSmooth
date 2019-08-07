#pragma once


int16_t current_working_state;

//  void automation_setup(){};
//  void automation_loop(){};


//defined in esphome.  be invoked out of esphome.
#include "esphome/components/logger/logger.h"
extern esphome::logger::Logger *logger_logger;
extern void esphome_pre_setup();
extern void esphome_setup();
extern void esphome_loop();



