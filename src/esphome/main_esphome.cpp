#include "esphome.h"
#include "main_esphome.h"

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
  wifi_wificomponent->set_use_address("smoothie.local");
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

mqtt::MQTTClientComponent *mqtt_mqttclientcomponent;

void setup_mqtt_broker(){
  mqtt_mqttclientcomponent = new mqtt::MQTTClientComponent();
  App.register_component(mqtt_mqttclientcomponent);
  //mqtt_mqttclientcomponent->set_broker_address("192.168.123.3");
  mqtt_mqttclientcomponent->set_broker_address("voicevon.vicp.io");
  mqtt_mqttclientcomponent->set_broker_port(1883);
  mqtt_mqttclientcomponent->set_username("von");
  mqtt_mqttclientcomponent->set_password("von1970");
  mqtt_mqttclientcomponent->set_discovery_info("homeassistant", true);
  mqtt_mqttclientcomponent->set_topic_prefix("smoothie");
  mqtt_mqttclientcomponent->set_birth_message(mqtt::MQTTMessage{
      .topic = "smoothie/status",
      .payload = "online",
      .qos = 0,
      .retain = true,
  });
  mqtt_mqttclientcomponent->set_last_will(mqtt::MQTTMessage{
      .topic = "smoothie/status",
      .payload = "offline",
      .qos = 0,
      .retain = true,
  });
  mqtt_mqttclientcomponent->set_shutdown_message(mqtt::MQTTMessage{
      .topic = "smoothie/status",
      .payload = "offline",
      .qos = 0,
      .retain = true,
  });
  mqtt_mqttclientcomponent->set_log_message_template(mqtt::MQTTMessage{
      .topic = "smoothie/debug",
      .payload = "",
      .qos = 0,
      .retain = true,
  });
  mqtt_mqttclientcomponent->set_keep_alive(15);
  mqtt_mqttclientcomponent->set_reboot_timeout(900000);
}


wifi_signal::WiFiSignalSensor *wifi_signal_wifisignalsensor;
sensor::SlidingWindowMovingAverageFilter *sensor_slidingwindowmovingaveragefilter;
mqtt::MQTTSensorComponent *mqtt_wifi_signal_strenth;

void setup_wifi_signal(){
  wifi_signal_wifisignalsensor = new wifi_signal::WiFiSignalSensor();
  wifi_signal_wifisignalsensor->set_update_interval(60000);

  App.register_component(wifi_signal_wifisignalsensor);
  App.register_sensor(wifi_signal_wifisignalsensor);
  wifi_signal_wifisignalsensor->set_name("wifi_signal");
  wifi_signal_wifisignalsensor->set_unit_of_measurement("dB");
  wifi_signal_wifisignalsensor->set_icon("mdi:wifi");
  wifi_signal_wifisignalsensor->set_accuracy_decimals(0);
  sensor_slidingwindowmovingaveragefilter = new sensor::SlidingWindowMovingAverageFilter(15, 15, 1);
  wifi_signal_wifisignalsensor->set_filters({sensor_slidingwindowmovingaveragefilter});
  mqtt_wifi_signal_strenth = new mqtt::MQTTSensorComponent(wifi_signal_wifisignalsensor);
  App.register_component(mqtt_wifi_signal_strenth);
}

uptime::UptimeSensor *uptime_sensor;
mqtt::MQTTSensorComponent *mqtt_uptime;

void setup_sensor_uptime(){
  uptime_sensor = new uptime::UptimeSensor();
  uptime_sensor->set_update_interval(10000);
  App.register_component(uptime_sensor);
  App.register_sensor(uptime_sensor);
  uptime_sensor->set_name("uptime Sensor");
  uptime_sensor->set_unit_of_measurement("s");
  uptime_sensor->set_icon("mdi:timer");
  uptime_sensor->set_accuracy_decimals(0);
  mqtt_uptime = new mqtt::MQTTSensorComponent(uptime_sensor);
  App.register_component(mqtt_uptime);
}

mqtt_subscribe::MQTTSubscribeSensor *working_mode_command;
mqtt::MQTTSensorComponent *mqtt_working_mode_command; 

//mqtt::MQTTClientComponent *mqtt_mqttclientcomponent;

void setup_sensor_mqtt_subscriber_working_mode(){
  working_mode_command = new mqtt_subscribe::MQTTSubscribeSensor();
  App.register_component(working_mode_command);
  App.register_sensor(working_mode_command);
  working_mode_command->set_name("working_mode");
  working_mode_command->set_unit_of_measurement("");
  working_mode_command->set_icon("");
  working_mode_command->set_accuracy_decimals(0);
  mqtt_working_mode_command = new mqtt::MQTTSensorComponent(working_mode_command);
  App.register_component(mqtt_working_mode_command);

  
  working_mode_command->set_parent(mqtt_mqttclientcomponent);
  working_mode_command->set_topic("smoothie/working_mode/command");
  working_mode_command->set_qos(0);
}

ramsensor::RamSensorInt16* current_working_state_sensor;
mqtt::MQTTSensorComponent *mqtt_working_state_sensor;

void setup_int_sensor_workingstate(){
  current_working_state_sensor = new ramsensor::RamSensorInt16();
  App.register_component(current_working_state_sensor);
  App.register_sensor(current_working_state_sensor);
  current_working_state_sensor->set_name("working_state");
  current_working_state_sensor->set_unit_of_measurement("");
  current_working_state_sensor->set_icon("");
  current_working_state_sensor->set_accuracy_decimals(0);
  current_working_state_sensor->set_int_source(&current_working_state);
  mqtt_working_state_sensor = new mqtt::MQTTSensorComponent(current_working_state_sensor);
  App.register_component(mqtt_working_state_sensor);
}

void esphome_setup() {
    App.pre_setup("smoothie", __DATE__ ", " __TIME__);
    setup_logger();
    setup_wifi();
    setup_mqtt_broker();
    setup_wifi_signal();
    setup_sensor_uptime();
    setup_sensor_mqtt_subscriber_working_mode();
    setup_int_sensor_workingstate();
    App.setup();
}


void esphome_loop() {
  App.loop();
}