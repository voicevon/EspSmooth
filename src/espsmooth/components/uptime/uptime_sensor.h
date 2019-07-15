#pragma once

#include "espsmooth/core/component.h"
#include "espsmooth/components/sensor/sensor.h"

namespace espsmooth {
namespace uptime {

class UptimeSensor : public sensor::Sensor, public PollingComponent {
 public:
  void update() override;

  float get_setup_priority() const override;

  std::string unique_id() override;

 protected:
  uint64_t uptime_{0};
};

}  // namespace uptime
}  // namespace espsmooth
