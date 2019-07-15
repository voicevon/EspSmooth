#pragma once

#include "espsmooth/core/component.h"
#include "espsmooth/core/helpers.h"
#include "espsmooth/components/sensor/sensor.h"
#include "espsmooth/components/wifi/wifi_component.h"

namespace espsmooth {
namespace wifi_signal {

class WiFiSignalSensor : public sensor::Sensor, public PollingComponent {
 public:
  void update() override { this->publish_state(WiFi.RSSI()); }
  void dump_config() override;

  std::string unique_id() override { return get_mac_address() + "-wifisignal"; }
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
};

}  // namespace wifi_signal
}  // namespace espsmooth
