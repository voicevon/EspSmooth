#pragma once
#include "_sal/configure/ConfigReader.h"
#include "esphome/components/ads1115/ads1115.h"



extern void Ads1115_setup(ConfigReader cr);
extern float Ads1115_read_sensor_mv(int chip_id, esphome::ads1115::ADS1115Multiplexer channel);




