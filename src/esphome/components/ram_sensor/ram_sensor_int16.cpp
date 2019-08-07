
#include "ram_sensor_int16.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/esphome.h"



namespace esphome {
namespace ramsensor {


static const char *TAG = "ram_sensor_int16";

void RamSensorInt16::setup(){
    this->state = 0;
    ESP_LOGCONFIG(TAG, "Setting up INT_SENSOR '%s'...", this->name_.c_str());
}

void RamSensorInt16::set_int_source(int16_t* target){
  this->pInt16 = target;
}

void RamSensorInt16::loop(){
  if (int16_t(this->state) != *this->pInt16){
    this->state = *this->pInt16;
    this->publish_state(this->state);
  }
}

}  // namespace intsensor
}  // namespace esphome