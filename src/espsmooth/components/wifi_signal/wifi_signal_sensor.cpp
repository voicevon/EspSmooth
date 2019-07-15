#include "wifi_signal_sensor.h"
#include "espsmooth/core/log.h"

namespace espsmooth {
namespace wifi_signal {

static const char *TAG = "wifi_signal.sensor";

void WiFiSignalSensor::dump_config() { LOG_SENSOR("", "WiFi Signal", this); }

}  // namespace wifi_signal
}  // namespace espsmooth
