#pragma once

#include "core/component.h"
#include "core/helpers.h"

namespace esphome {
namespace sensor {

#define LOG_SENSOR(prefix, type, obj) \
  if (obj != nullptr) { \
    ESP_LOGCONFIG(TAG, prefix type " '%s'", obj->get_name().c_str()); \
    ESP_LOGCONFIG(TAG, prefix "  Accuracy Decimals: %d", obj->get_accuracy_decimals()); \
      if (!obj->unique_id().empty()) { \
      ESP_LOGV(TAG, prefix "  Unique ID: '%s'", obj->unique_id().c_str()); \
    } \
  }

/** Base-class for all sensors.
 *
 * A sensor has unit of measurement and can use publish_state to send out a new value with the specified accuracy.
 */
class Sensor : public Nameable {
 public:
  explicit Sensor();
  explicit Sensor(const std::string &name);

  /** Manually set the accuracy in decimals for this sensor. By default, the sensor's default defined by
   * accuracy_decimals() is used.
   *
   * @param accuracy_decimals The accuracy decimal that should be used.
   */
  void set_accuracy_decimals(int8_t accuracy_decimals);

  /// Getter-syntax for .state.
  float get_state() const;
  /// Getter-syntax for .raw_value. Please use .raw_state instead.
  float get_raw_value() const ESPDEPRECATED(".raw_value is deprecated, please use .raw_state");
  /// Getter-syntax for .raw_state
  float get_raw_state() const;

  /// Get the accuracy in decimals. Uses the manual override if specified or the default value instead.
  int8_t get_accuracy_decimals();




  /** Publish a new state to the front-end.
   *
   * First, the new state will be assigned to the raw_value. Then it's passed through all filters
   * until it finally lands in the .value member variable and a callback is issued.
   *
   * @param state The state as a floating point number.
   */
  void publish_state(float state);

  /** Push a new value to the MQTT front-end.
   *
   * Note: deprecated, please use publish_state.
   */
  void push_new_value(float state) ESPDEPRECATED("push_new_value is deprecated. Please use .publish_state instead");

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  /// Add a callback that will be called every time a filtered value arrives.
  void add_on_state_callback(std::function<void(float)> &&callback);
  /// Add a callback that will be called every time the sensor sends a raw value.
  void add_on_raw_state_callback(std::function<void(float)> &&callback);

  /** This member variable stores the last state that has passed through all filters.
   *
   * On startup, when no state is available yet, this is NAN (not-a-number) and the validity
   * can be checked using has_state().
   *
   * This is exposed through a member variable for ease of use in esphome lambdas.
   */
  float state;

  /** This member variable stores the current raw state of the sensor. Unlike .state,
   * this will be updated immediately when publish_state is called.
   */
  float raw_state;

  /// Return whether this sensor has gotten a full state (that passed through all filters) yet.
  bool has_state() const;

  /** A unique ID for this sensor, empty for no unique id. See unique ID requirements:
   * https://developers.home-assistant.io/docs/en/entity_registry_index.html#unique-id-requirements
   *
   * @return The unique id as a string.
   */
  virtual std::string unique_id();

  /// Return with which interval the sensor is polled. Return 0 for non-polling mode.
  virtual uint32_t update_interval();

  /// Calculate the expected update interval for values that pass through all filters.
  uint32_t calculate_expected_filter_update_interval();

  void internal_send_state_to_frontend(float state);

 protected:
 
  /// Return the accuracy in decimals for this sensor.
  virtual int8_t accuracy_decimals();  // NOLINT

  uint32_t hash_base() override;

  CallbackManager<void(float)> raw_callback_;  ///< Storage for raw state callbacks.
  CallbackManager<void(float)> callback_;      ///< Storage for filtered state callbacks.
  /// Override the accuracy in decimals, otherwise the sensor's values will be used.
  optional<int8_t> accuracy_decimals_;
  // Filter *filter_list_{nullptr};  ///< Store all active filters.
  bool has_state_{false};
};

class PollingSensorComponent : public PollingComponent, public Sensor {
 public:
  explicit PollingSensorComponent(const std::string &name, uint32_t update_interval);

  uint32_t update_interval() override;
};

}  // namespace sensor
}  // namespace esphome
