#pragma once
#include "_sal/configure/ConfigReader.h"
#include "esphome/esphome.h"

static const int CHIP_COUNT = 1;
static const int CHANNEL_COUNT = 4; 

class Ads1115{
    public:
        static Ads1115* get_instance(){ return __instance; }
        void Ads1115_setup(ConfigReader cr);
        float Ads1115_read_sensor_mv(int chip_id,int channel);

    private:
        Ads1115();
        static Ads1115* __instance;
        esphome::i2c::I2CComponent * i2c_i2ccomponent;
        esphome::ads1115::ADS1115Component *ads1115_ads1115component[CHIP_COUNT];
        esphome::ads1115::ADS1115Sensor *ads1115_ads1115sensor[CHIP_COUNT][CHANNEL_COUNT];

};

