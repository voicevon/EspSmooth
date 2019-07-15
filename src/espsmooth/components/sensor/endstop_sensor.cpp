
#include "endstop_sensor.h"
#include "espsmooth/core/log.h"

namespace espsmooth {
namespace endstop {

static const char *TAG = "endstop";

void EndstopSensor::update() {
  ESP_LOGCONFIG(TAG, "Setting up ADC '%s'...", this->get_name().c_str());
//   GPIOPin(this->pin_, INPUT).setup();
}

// float EndstopSensor::get_setup_priority(){
//   return 0.1f;
// }

}  // namespace endstop
}  // namespace espsmooth