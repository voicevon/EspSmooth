// #include "Ads1115.h"
#include "_sal/configure/ConfigReader.h"
#include "esphome/esphome.h"

using namespace esphome;
using namespace i2c;

static const int CHIP_COUNT = 1;
static const int CHANNEL_COUNT = 4; 


        esphome::i2c::I2CComponent * i2c_i2ccomponent;
        esphome::ads1115::ADS1115Component *ads1115_ads1115component[CHIP_COUNT];
        esphome::ads1115::ADS1115Sensor *ads1115_ads1115sensor[CHIP_COUNT][CHANNEL_COUNT];


void Ads1115_setup(ConfigReader cr){
  printf(" aaaaaaaaa \n");
  i2c_i2ccomponent = new i2c::I2CComponent();
  i2c_i2ccomponent->set_sda_pin(27);
  i2c_i2ccomponent->set_scl_pin(14);
  i2c_i2ccomponent->set_frequency(400000);
  i2c_i2ccomponent->set_scan(true);
  i2c_i2ccomponent->setup();

  for(int i=0; i<CHIP_COUNT; i++){
  printf(" bbbbbbbbbb %i\n",i);
    ads1115_ads1115component[i] = new ads1115::ADS1115Component();
    ads1115_ads1115component[i]->set_i2c_parent(i2c_i2ccomponent);
    if(i==0) ads1115_ads1115component[i]->set_i2c_address(0x48);
    if(i==1) ads1115_ads1115component[i]->set_i2c_address(0x49);
    if(i==2) ads1115_ads1115component[i]->set_i2c_address(0x4a);
    if(i==3) ads1115_ads1115component[i]->set_i2c_address(0x4b);
    ads1115_ads1115component[i]->setup();

    for(int j=0; j<CHANNEL_COUNT; j++){
      printf(" ccccccccc %i,%i\n",i,j);
      ads1115_ads1115sensor[i][j] = new ads1115::ADS1115Sensor(ads1115_ads1115component[i]);
      ads1115_ads1115component[i]->register_sensor(ads1115_ads1115sensor[i][j]);
      if(j==0) ads1115_ads1115sensor[i][j]->set_multiplexer(ads1115::ADS1115_MULTIPLEXER_P0_NG); 
      if(j==1) ads1115_ads1115sensor[i][j]->set_multiplexer(ads1115::ADS1115_MULTIPLEXER_P1_NG); 
      if(j==2) ads1115_ads1115sensor[i][j]->set_multiplexer(ads1115::ADS1115_MULTIPLEXER_P2_NG); 
      if(j==3) ads1115_ads1115sensor[i][j]->set_multiplexer(ads1115::ADS1115_MULTIPLEXER_P3_NG); 
      ads1115_ads1115sensor[i][j]->set_gain(ads1115::ADS1115_GAIN_6P144);               
      // ads1115_ads1115sensor[0][0]->set_name("alpha_pos");
      // ads1115_ads1115sensor[0][0]->set_unit_of_measurement("");
      // ads1115_ads1115sensor[0][0]->set_icon("");
      // ads1115_ads1115sensor[0][0]->set_accuracy_decimals(3);
      // ads1115_ads1115sensor[0][0]->set_update_interval(1000);  //from component? or pulling_sensor?
      ads1115_ads1115sensor[i][j]->setup();
    }
  }
}

//  Stack size is too small!
//  https://esp32.com/viewtopic.php?t=1459
float Ads1115_read_sensor_mv(int chip_id,int channel){
  // printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ %i,%i\n",chip_id,channel);
    // ads1115_ads1115sensor[chip_id][channel]->update();
    return ads1115_ads1115component[chip_id]->request_measurement(ads1115_ads1115sensor[chip_id][channel]);
    // return ads1115_ads1115sensor[chip_id][channel]->state;
}
