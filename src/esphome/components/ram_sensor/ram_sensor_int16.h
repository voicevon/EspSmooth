#pragma once

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"
#include "esphome/components/sensor/sensor.h"


//extern int current_working_state;


namespace esphome {
namespace ramsensor {



class RamSensorInt16 : public Component, public sensor::Sensor {
 public:
  void setup() override;
  void set_int_source(int16_t* target);
  void loop() override;
 
 private:
  int16_t* pInt16;
};

}  // namespace intsensor
}  // namespace esphome

