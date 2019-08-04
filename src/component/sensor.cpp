#include "sensor.h"
#include "core/log.h"

namespace esphome {
namespace sensor {

static const char *TAG = "sensor";

void Sensor::publish_state(float state) {
  this->raw_state = state;
  this->raw_callback_.call(state);

  ESP_LOGV(TAG, "'%s': Received new state %f", this->name_.c_str(), state);

  // if (this->filter_list_ == nullptr) {
    this->internal_send_state_to_frontend(state);
  // } else {
  //   this->filter_list_->input(state);
  // }
}
void Sensor::push_new_value(float state) { this->publish_state(state); }
uint32_t Sensor::update_interval() { return 0; }
int8_t Sensor::accuracy_decimals() { return 0; }
Sensor::Sensor(const std::string &name) : Nameable(name), state(NAN), raw_state(NAN) {}
Sensor::Sensor() : Sensor("") {}

void Sensor::set_accuracy_decimals(int8_t accuracy_decimals) { this->accuracy_decimals_ = accuracy_decimals; }
void Sensor::add_on_state_callback(std::function<void(float)> &&callback) { this->callback_.add(std::move(callback)); }
void Sensor::add_on_raw_state_callback(std::function<void(float)> &&callback) {
  this->raw_callback_.add(std::move(callback));
}

int8_t Sensor::get_accuracy_decimals() {
  if (this->accuracy_decimals_.has_value())
    return *this->accuracy_decimals_;
  return this->accuracy_decimals();
}

float Sensor::get_value() const { return this->state; }
float Sensor::get_state() const { return this->state; }
float Sensor::get_raw_value() const { return this->raw_state; }
float Sensor::get_raw_state() const { return this->raw_state; }
std::string Sensor::unique_id() { return ""; }

void Sensor::internal_send_state_to_frontend(float state) {
  this->has_state_ = true;
  this->state = state;
  ESP_LOGD(TAG, "'%s': Sending state %.5f with %d decimals of accuracy", this->get_name().c_str(), state,
            this->get_accuracy_decimals());
  this->callback_.call(state);
}
bool Sensor::has_state() const { return this->has_state_; }
uint32_t Sensor::calculate_expected_filter_update_interval() {
  uint32_t interval = this->update_interval();
  if (interval == 4294967295UL)
    // update_interval: never
    return 0;

  // if (this->filter_list_ == nullptr) {
    return interval;
  // }

  // return this->filter_list_->calculate_remaining_interval(interval);
}
uint32_t Sensor::hash_base() { return 2455723294UL; }

PollingSensorComponent::PollingSensorComponent(const std::string &name, uint32_t update_interval)
    : PollingComponent(update_interval), Sensor(name) {}

uint32_t PollingSensorComponent::update_interval() { return this->get_update_interval(); }

}  // namespace sensor
}  // namespace esphome
