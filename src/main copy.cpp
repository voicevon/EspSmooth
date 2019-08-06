#include "esphome.h"
using namespace esphome;


float g_value_of_zero_mass = 8388608.0f;
float g_slope = 0.000123;
// result = (value - g_value_of_zero_mass) * slope









logger::Logger *logger_logger;

void setup_logger(){
  logger_logger = new logger::Logger(115200, 512, logger::UART_SELECTION_UART0);
  logger_logger->pre_setup();
  // logger_logger->set_log_level("",ESPHOME_LOG_LEVEL_INFO);
  logger_logger->set_log_level("sensor", ESPHOME_LOG_LEVEL_INFO);
  logger_logger->set_log_level("adc", ESPHOME_LOG_LEVEL_INFO);
  logger_logger->set_log_level("pulse_counter", ESPHOME_LOG_LEVEL_INFO);
  logger_logger->set_log_level("switch", ESPHOME_LOG_LEVEL_INFO);
  logger_logger->set_log_level("mqtt", ESPHOME_LOG_LEVEL_INFO);
  App.register_component(logger_logger);
}

wifi::WiFiComponent *wifi_wificomponent;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

void setup_wifi(){
  wifi_wificomponent = new wifi::WiFiComponent();
  wifi_wificomponent->set_use_address("water.local");
  wifi::WiFiAP wifi_wifiap = wifi::WiFiAP();
  wifi_wifiap.set_ssid("FuckGFW");
  wifi_wifiap.set_password("refuckgfw");
  wifi_wifiap.set_priority(0.000000f);
  wifi_wificomponent->add_sta(wifi_wifiap);
  wifi_wificomponent->set_reboot_timeout(900000);
  wifi_wificomponent->set_power_save_mode(wifi::WIFI_POWER_SAVE_LIGHT);
  wifi_wificomponent->set_fast_connect(false);
  App.register_component(wifi_wificomponent);
}


void setup() {
  App.pre_setup("water", __DATE__ ", " __TIME__);
  setup_logger();
  setup_wifi();
  
//   setup4_template(); 
  // setup5_display();
  App.setup();
}


void loop() {
  App.loop();
}