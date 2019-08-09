
#include "esphome/esphome.h"

using namespace esphome;
// extern wifi::WiFiComponent *wifi_wificomponent;
// extern mqtt::MQTTClientComponent *mqtt_mqttclientcomponent;
// extern esphome::logger::Logger *logger_logger;
// extern void esphome_setup();
// using namespace mqtt;

using namespace i2c;


// void setup_logger(){
//   logger_logger = new logger::Logger(115200, 512, logger::UART_SELECTION_UART0);
//   logger_logger->pre_setup();
//   App.register_component(logger_logger);
  
// }
// void setup_wifi_mqtt()
// {
  
//   wifi_wificomponent = new wifi::WiFiComponent();
//   wifi_wificomponent->set_use_address("smoothie.local");
//   wifi::WiFiAP wifi_wifiap = wifi::WiFiAP();
//   wifi_wifiap.set_ssid("FuckGFW");
//   wifi_wifiap.set_password("refuckgfw");
//   wifi_wificomponent->add_sta(wifi_wifiap);
//   wifi_wificomponent->set_reboot_timeout(300000);
//   wifi_wificomponent->set_power_save_mode(wifi::WIFI_POWER_SAVE_NONE);
//   wifi_wificomponent->set_fast_connect(false);
//   App.register_component(wifi_wificomponent);

//   mqtt_mqttclientcomponent = new mqtt::MQTTClientComponent();
//   App.register_component(mqtt_mqttclientcomponent);
//   // sensor:
//   mqtt_mqttclientcomponent->set_broker_address("voicevon.vicp.io");
//   mqtt_mqttclientcomponent->set_broker_port(1883);
//   mqtt_mqttclientcomponent->set_username("von");
//   mqtt_mqttclientcomponent->set_password("von1970");
//   mqtt_mqttclientcomponent->set_discovery_info("homeassistant", true);
//   mqtt_mqttclientcomponent->set_topic_prefix("smoothie");
//   mqtt_mqttclientcomponent->set_birth_message(mqtt::MQTTMessage{
//       .topic = "smoothie/status",
//       .payload = "online",
//       .qos = 0,
//       .retain = true,
//   });
//   mqtt_mqttclientcomponent->set_last_will(mqtt::MQTTMessage{
//       .topic = "smoothie/status",
//       .payload = "offline",
//       .qos = 0,
//       .retain = true,
//   });
//   mqtt_mqttclientcomponent->set_shutdown_message(mqtt::MQTTMessage{
//       .topic = "smoothie/status",
//       .payload = "offline",
//       .qos = 0,
//       .retain = true,
//   });
//   mqtt_mqttclientcomponent->set_log_message_template(mqtt::MQTTMessage{
//       .topic = "smoothie/debug",
//       .payload = "",
//       .qos = 0,
//       .retain = true,
//   });
//   mqtt_mqttclientcomponent->set_keep_alive(15);
//   mqtt_mqttclientcomponent->set_reboot_timeout(300000);

// }
// mqtt::MQTTSensorComponent *mqtt_mqttsensorcomponent;

i2c::I2CComponent *i2c_i2ccomponent;
ads1115::ADS1115Component *ads1115_ads1115component;
ads1115::ADS1115Sensor *ads1115_ads1115sensor;
void setup_ads1115(){
  
  i2c_i2ccomponent = new i2c::I2CComponent();
  ads1115_ads1115component = new ads1115::ADS1115Component();
  ads1115_ads1115sensor = new ads1115::ADS1115Sensor(ads1115_ads1115component);
  // App.register_component(i2c_i2ccomponent);
  // App.register_component(ads1115_ads1115component);
  // App.register_component(ads1115_ads1115sensor);
  // App.register_sensor(ads1115_ads1115sensor);


  ads1115_ads1115component->register_sensor(ads1115_ads1115sensor);

  i2c_i2ccomponent->set_sda_pin(27);
  i2c_i2ccomponent->set_scl_pin(14);
  i2c_i2ccomponent->set_frequency(400000);
  i2c_i2ccomponent->set_scan(true);


  ads1115_ads1115sensor->set_name("alpha_pos");
  ads1115_ads1115sensor->set_unit_of_measurement("");
  ads1115_ads1115sensor->set_icon("");
  ads1115_ads1115sensor->set_accuracy_decimals(3);


  ads1115_ads1115component->set_i2c_parent(i2c_i2ccomponent);
  ads1115_ads1115component->set_i2c_address(0x48);
  
  ads1115_ads1115sensor->set_update_interval(1000);  //from component?
  ads1115_ads1115sensor->set_multiplexer(ads1115::ADS1115_MULTIPLEXER_P1_NG); //from sensor
  ads1115_ads1115sensor->set_gain(ads1115::ADS1115_GAIN_6P144);               //from sensor
  
  i2c_i2ccomponent->setup();
  ads1115_ads1115component->setup();
  ads1115_ads1115sensor->setup();
  
}

// void setup2() {

//   App.pre_setup("smoothie", __DATE__ ", " __TIME__);
//   setup_logger();
//   // setup_wifi_mqtt();
//   setup_ads1115();
//   App.setup();
  // logger_logger->set_log_level("sensor",ESPHOME_LOG_LEVEL_ERROR);
  // logger_logger->set_log_level("ads1115",ESPHOME_LOG_LEVEL_ERROR);
// }
// int i;
// void loop2() {
//   // App.loop();
//   ads1115_ads1115sensor->update();
//   float xx= ads1115_ads1115sensor->state;
//   printf(" %f", xx);
//   i++;
//   if (i==10) {
//     i=0;
//     printf("\n");
//   }
//   delay(2);
// }
