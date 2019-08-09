#pragma once
#include "_sal/configure/ConfigReader.h"

extern void Ads1115_setup(ConfigReader cr);
extern float Ads1115_read_sensor_mv(int chip_id,int channel);
