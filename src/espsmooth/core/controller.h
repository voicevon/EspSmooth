#pragma once

#include "espsmooth/core/defines.h"
#ifdef USE_BINARY_SENSOR
#include "espsmooth/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_FAN
#include "espsmooth/components/fan/fan_state.h"
#endif
#ifdef USE_LIGHT
#include "espsmooth/components/light/light_state.h"
#endif
#ifdef USE_COVER
#include "espsmooth/components/cover/cover.h"
#endif
#ifdef USE_SENSOR
#include "espsmooth/components/sensor/sensor.h"
#endif
#ifdef USE_TEXT_SENSOR
#include "espsmooth/components/text_sensor/text_sensor.h"
#endif
#ifdef USE_SWITCH
#include "espsmooth/components/switch/switch.h"
#endif
#ifdef USE_CLIMATE
#include "espsmooth/components/climate/climate.h"
#endif

namespace espsmooth {

class Controller {
 public:
  void setup_controller();
#ifdef USE_BINARY_SENSOR
  virtual void on_binary_sensor_update(binary_sensor::BinarySensor *obj, bool state){};
#endif
#ifdef USE_FAN
  virtual void on_fan_update(fan::FanState *obj){};
#endif
#ifdef USE_LIGHT
  virtual void on_light_update(light::LightState *obj){};
#endif
#ifdef USE_SENSOR
  virtual void on_sensor_update(sensor::Sensor *obj, float state){};
#endif
#ifdef USE_SWITCH
  virtual void on_switch_update(switch_::Switch *obj, bool state){};
#endif
#ifdef USE_COVER
  virtual void on_cover_update(cover::Cover *obj){};
#endif
#ifdef USE_TEXT_SENSOR
  virtual void on_text_sensor_update(text_sensor::TextSensor *obj, std::string state){};
#endif
#ifdef USE_CLIMATE
  virtual void on_climate_update(climate::Climate *obj){};
#endif
};

}  // namespace espsmooth
