#include "sensor.h"
#include "core2/log.h"

namespace esphome {
namespace sensor {

static const char *TAG = "sensor";

void Sensor::publish_state(float state) {
  this->raw_state = state;

  ESP_LOGV(TAG, "'%s': Received new state %f", this->name_.c_str(), state);

    this->internal_send_state_to_frontend(state);
}
void Sensor::push_new_value(float state) { this->publish_state(state); }
uint32_t Sensor::update_interval() { return 0; }
int8_t Sensor::accuracy_decimals() { return 0; }
Sensor::Sensor(const std::string &name) : Nameable(name), state(NAN), raw_state(NAN) {}
Sensor::Sensor() : Sensor("") {}

// void Sensor::set_accuracy_decimals(int8_t accuracy_decimals) { this->accuracy_decimals_ = accuracy_decimals; }


// int8_t Sensor::get_accuracy_decimals() {
//   if (this->accuracy_decimals_.has_value())
//     return *this->accuracy_decimals_;
//   return this->accuracy_decimals();
// }

float Sensor::get_state() const { return this->state; }
float Sensor::get_raw_state() const { return this->raw_state; }


void Sensor::internal_send_state_to_frontend(float state) {
  this->has_state_ = true;
  this->state = state;
  // ESP_LOGD(TAG, "'%s': Sending state %.5f with %d decimals of accuracy", this->get_name().c_str(), state,
  //           this->get_accuracy_decimals());
  
}
bool Sensor::has_state() const { return this->has_state_; }

PollingSensorComponent::PollingSensorComponent(const std::string &name, uint32_t update_interval)
    : PollingComponent(update_interval), Sensor(name) {}

uint32_t PollingSensorComponent::update_interval() { return this->get_update_interval(); }

}  // namespace sensor
}  // namespace esphome
