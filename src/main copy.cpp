#include "esphome.h"
#include "main.h"
using namespace esphome;


//sensor::HX711Sensor* weight;


gpio::GPIOBinarySensor* bs_door;
// template_::TemplateBinarySensor * leaking;

// next version:  a component named weighing_scale_hx711
// action_1 :=  reset zero mass value (long new_value)
// action_2 :=  reset second mass and value (float second_mess, long new_value) , will change the g_slope

float g_value_of_zero_mass = 8388608.0f;
float g_slope = 0.000123;
// result = (value - g_value_of_zero_mass) * slope


void setup_apps_ondebug(){
  //sensor::HX711
  // weight = App.make_hx711_sensor("weight", 22, 21, 20000);
  // weight->set_gain(sensor::HX711_GAIN_128);
  // weight->set_unit_of_measurement("Kg");
  // weight->set_filters({
  //     new sensor::LambdaFilter([=](float x) {
  //       auto first_mass = 0.0; // first known mass was 0kg
  //       auto first_value = g_value_of_zero_mass; // value for the first known mass was 120
  //       auto second_mass = 1000.0; // second mass was 1kg
  //       auto second_value = 8588609; // second value was 810
  //       return map(x, first_value, second_value, first_mass, second_mass);
  //   }),
  // });
}

adc::ADCSensor* adc_tds;
mqtt::MQTTSensorComponent *mqtt_tds;

void setup_gpio_adc_tds(){
  adc_tds = new adc::ADCSensor();
  adc_tds->set_update_interval(10000);
  App.register_component(adc_tds);

  App.register_sensor(adc_tds);
  adc_tds->set_name("tds");
  adc_tds->set_unit_of_measurement("ppm");
  adc_tds->set_icon("mdi:flash");
  adc_tds->set_accuracy_decimals(2);
  adc_tds->set_pin(39);
  adc_tds->set_attenuation(ADC_11db);
  sensor::SlidingWindowMovingAverageFilter *sensor_slidingwindowmovingaveragefilter;
  sensor_slidingwindowmovingaveragefilter = new sensor::SlidingWindowMovingAverageFilter(15, 15, 1);
  
  adc_tds->set_filters({
      sensor_slidingwindowmovingaveragefilter,
      new sensor::LambdaFilter([=](float x) -> optional<float> {
        float x2 = 0.54f;
        float y2 = 371.0f;
        float x1 = 0.08f;
        float y1 = 124.0f;
        float s = (y2 - y1) / (x2 - x1);
        float c = y1 - s * x1;
        return x * s + c;
    }),
  });
  mqtt_tds = new mqtt::MQTTSensorComponent(adc_tds);
  App.register_component(mqtt_tds);
}

adc::ADCSensor* adc_leaking;
mqtt::MQTTSensorComponent *mqtt_adc_leaking;

void setup_gpio_adc_leaking(){
  adc_leaking = new adc::ADCSensor();
  adc_leaking->set_update_interval(10000);
  App.register_component(adc_leaking);

  App.register_sensor(adc_leaking);
  adc_leaking->set_name("adc_leaking");
  adc_leaking->set_unit_of_measurement("V");
  adc_leaking->set_icon("mdi:flash");
  adc_leaking->set_accuracy_decimals(2);
  adc_leaking->set_pin(36);
  adc_leaking->set_attenuation(ADC_11db);
  sensor::SlidingWindowMovingAverageFilter *sensor_slidingwindowmovingaveragefilter;
  sensor_slidingwindowmovingaveragefilter = new sensor::SlidingWindowMovingAverageFilter(15, 15, 1);
  
  mqtt_adc_leaking = new mqtt::MQTTSensorComponent(adc_leaking);
  App.register_component(mqtt_adc_leaking);
}


adc::ADCSensor* adc_ap;    //膜前压力
mqtt::MQTTSensorComponent *mqtt_aPressure;

void setup_gpio_adc_aPressure(){
  adc_ap = new adc::ADCSensor();
  adc_ap->set_update_interval(10000);
  App.register_component(adc_ap);

  App.register_sensor(adc_ap);
  adc_ap->set_name("ap");
  adc_ap->set_unit_of_measurement("kg");
  adc_ap->set_icon("mdi:flash");
  adc_ap->set_accuracy_decimals(2);
  adc_ap->set_pin(33);
  adc_ap->set_attenuation(ADC_11db);
  sensor::SlidingWindowMovingAverageFilter *sensor_slidingwindowmovingaveragefilter;
  sensor_slidingwindowmovingaveragefilter = new sensor::SlidingWindowMovingAverageFilter(5, 5, 1);
  
  adc_ap->set_filters({
      sensor_slidingwindowmovingaveragefilter,
      new sensor::LambdaFilter([=](float x) -> optional<float> {
        float x2 = 3.8f;
        float y2 = 8.1f;
        float x1 = 0.38f;
        float y1 = 0.0f;
        float s = (y2 - y1) / (x2 - x1);
        float c = y1 - s * x1;
        return x * s + c;
    }),
  });
  mqtt_aPressure = new mqtt::MQTTSensorComponent(adc_ap);
  App.register_component(mqtt_aPressure);
}

adc::ADCSensor* adc_bp;    //净水压力
mqtt::MQTTSensorComponent *mqtt_bPressure;

void setup_gpio_adc_bp(){
  adc_bp = new adc::ADCSensor();
  adc_bp->set_update_interval(10000);
  App.register_component(adc_bp);

  App.register_sensor(adc_bp);
  adc_bp->set_name("bp");
  adc_bp->set_unit_of_measurement("kg");
  adc_bp->set_icon("mdi:flash");
  adc_bp->set_accuracy_decimals(2);
  adc_bp->set_pin(34);
  adc_bp->set_attenuation(ADC_11db);
  sensor::SlidingWindowMovingAverageFilter *sensor_slidingwindowmovingaveragefilter;
  sensor_slidingwindowmovingaveragefilter = new sensor::SlidingWindowMovingAverageFilter(2, 2, 1);
  
  adc_bp->set_filters({
      sensor_slidingwindowmovingaveragefilter,
      new sensor::LambdaFilter([=](float x) -> optional<float> {
        float x2 = 3.8f;
        float y2 = 8.1f;
        float x1 = 0.38f;
        float y1 = 0.0f;
        float s = (y2 - y1) / (x2 - x1);
        float c = y1 - s * x1;
        return x * s + c;
    }),
  });
  mqtt_bPressure = new mqtt::MQTTSensorComponent(adc_bp);
  App.register_component(mqtt_bPressure);
}


adc::ADCSensor* adc_sp;    //源水压力
mqtt::MQTTSensorComponent *mqtt_sPressure;

void setup_gpio_adc_sp(){
  adc_sp = new adc::ADCSensor();
  adc_sp->set_update_interval(10000);
  App.register_component(adc_sp);

  App.register_sensor(adc_sp);
  adc_sp->set_name("sp");
  adc_sp->set_unit_of_measurement("kg");
  adc_sp->set_icon("mdi:flash");
  adc_sp->set_accuracy_decimals(2);
  adc_sp->set_pin(35);
  adc_sp->set_attenuation(ADC_11db);
  sensor::SlidingWindowMovingAverageFilter *sensor_slidingwindowmovingaveragefilter;
  sensor_slidingwindowmovingaveragefilter = new sensor::SlidingWindowMovingAverageFilter(5, 5, 1);
  
  adc_sp->set_filters({
      sensor_slidingwindowmovingaveragefilter,
      new sensor::LambdaFilter([=](float x) -> optional<float> {
        float x2 = 3.8f;
        float y2 = 8.1f;
        float x1 = 0.38f;
        float y1 = 0.0f;
        float s = (y2 - y1) / (x2 - x1);
        float c = y1 - s * x1;
        return x * s + c;
    }),
  });
  mqtt_sPressure = new mqtt::MQTTSensorComponent(adc_sp);
  App.register_component(mqtt_sPressure);
}



pulse_counter::PulseCounterSensor * pc_source;  //原水流量
mqtt::MQTTSensorComponent *mqtt_flow_source;
sensor::MultiplyFilter *sensor_multiplyfilter_flow_source;

void setup_gpio_pulse_counter_source(){
  pc_source = new pulse_counter::PulseCounterSensor();
  //common
  pc_source->set_update_interval(10000);
  pc_source->set_name("pc_source");
  pc_source->set_unit_of_measurement("L");
  pc_source->set_icon("mdi:pulse");
  pc_source->set_accuracy_decimals(2);
  //hardware
  pc_source->set_pin(new GPIOPin(18, INPUT, false));
  pc_source->set_rising_edge_mode(pulse_counter::PULSE_COUNTER_INCREMENT);
  pc_source->set_falling_edge_mode(pulse_counter::PULSE_COUNTER_DISABLE);
  pc_source->set_filter_us(13);
  //filters
  sensor_multiplyfilter_flow_source = new sensor::MultiplyFilter(0.000485f);
  pc_source->set_filters({sensor_multiplyfilter_flow_source});
  //registering
  mqtt_flow_source = new mqtt::MQTTSensorComponent(pc_source);
  App.register_component(pc_source);
  App.register_sensor(pc_source);
  App.register_component(mqtt_flow_source);
}


pulse_counter::PulseCounterSensor * pc_inlet;  //入罐流量
mqtt::MQTTSensorComponent *mqtt_flow_inlet;
sensor::MultiplyFilter *sensor_multiplyfilter_flow_inlet;

pulse_counter::PulseCounterSensor * pc_c;  //罐后流量

void setup_gpio_pulse_counter_inlet(){
  pc_inlet = new pulse_counter::PulseCounterSensor();
  //common
  pc_inlet->set_update_interval(10000);
  pc_inlet->set_name("pc_inlet");
  pc_inlet->set_unit_of_measurement("L");
  pc_inlet->set_icon("mdi:pulse");
  pc_inlet->set_accuracy_decimals(2);
  //hardware
  pc_inlet->set_pin(new GPIOPin(16, INPUT, false));
  pc_inlet->set_rising_edge_mode(pulse_counter::PULSE_COUNTER_INCREMENT);
  pc_inlet->set_falling_edge_mode(pulse_counter::PULSE_COUNTER_DISABLE);
  pc_inlet->set_filter_us(13);
  //filters
  sensor_multiplyfilter_flow_inlet = new sensor::MultiplyFilter(0.000485f);
  pc_inlet->set_filters({sensor_multiplyfilter_flow_inlet});
  //registering
  mqtt_flow_inlet = new mqtt::MQTTSensorComponent(pc_inlet);
  App.register_component(pc_inlet);
  App.register_sensor(pc_inlet);
  App.register_component(mqtt_flow_inlet);
}

pulse_counter::PulseCounterSensor * pc_outlet;  //出罐流量
mqtt::MQTTSensorComponent *mqtt_flow_outlet;
sensor::MultiplyFilter *sensor_multiplyfilter_flow_outlet;

void setup_gpio_pulse_counter_outlet(){
  pc_outlet = new pulse_counter::PulseCounterSensor();
  //common
  pc_outlet->set_update_interval(10000);
  pc_outlet->set_name("pc_outlet");
  pc_outlet->set_unit_of_measurement("L");
  pc_outlet->set_icon("mdi:pulse");
  pc_outlet->set_accuracy_decimals(2);
  //hardware
  pc_outlet->set_pin(new GPIOPin(17, INPUT, false));
  pc_outlet->set_rising_edge_mode(pulse_counter::PULSE_COUNTER_INCREMENT);
  pc_outlet->set_falling_edge_mode(pulse_counter::PULSE_COUNTER_DISABLE);
  pc_outlet->set_filter_us(13);
  //filters
  sensor_multiplyfilter_flow_outlet = new sensor::MultiplyFilter(0.000485f);
  pc_outlet->set_filters({sensor_multiplyfilter_flow_outlet});
  //registering
  mqtt_flow_outlet = new mqtt::MQTTSensorComponent(pc_outlet);
  App.register_component(pc_outlet);
  App.register_sensor(pc_outlet);
  App.register_component(mqtt_flow_outlet);
}

gpio::GPIOBinarySensor *bin_door;
mqtt::MQTTBinarySensorComponent *mqtt_door;

void setup_gpio_binary_sensor_door(){
    bin_door = new gpio::GPIOBinarySensor();
    //basic
    bin_door->set_name("door");
    //hardware
    bin_door->set_pin(new GPIOPin(23, INPUT_PULLUP, false));
    mqtt_door = new mqtt::MQTTBinarySensorComponent(bin_door);

    App.register_component(bin_door);
    App.register_binary_sensor(bin_door);
    App.register_component(mqtt_door);
}

gpio::GPIOSwitch* r1; //增压泵1
mqtt::MQTTSwitchComponent *mqtt_r1;

void setup_gpio_switch_r1(){
  r1 = new gpio::GPIOSwitch();
  r1->set_name("r1");
  //hardware
  r1->set_pin(new GPIOPin(13, OUTPUT, false));
  r1->set_restore_mode(gpio::GPIO_SWITCH_RESTORE_DEFAULT_OFF);
  //register
  App.register_component(r1);
  App.register_switch(r1);
  mqtt_r1 = new mqtt::MQTTSwitchComponent(r1);
  App.register_component(mqtt_r1);
}

gpio::GPIOSwitch* r2; //增压泵2 
mqtt::MQTTSwitchComponent *mqtt_r2;

void setup_gpio_switch_r2(){
  r2 = new gpio::GPIOSwitch();
  r2->set_name("r2");
  //hardware
  r2->set_pin(new GPIOPin(32, OUTPUT, false));
  r2->set_restore_mode(gpio::GPIO_SWITCH_RESTORE_DEFAULT_OFF);
  //register
  App.register_component(r2);
  App.register_switch(r2);
  mqtt_r2 = new mqtt::MQTTSwitchComponent(r2);
  App.register_component(mqtt_r2);
}

gpio::GPIOSwitch* r3;  //源水电磁阀
mqtt::MQTTSwitchComponent *mqtt_r3;

void setup_gpio_switch_r3(){
  r3 = new gpio::GPIOSwitch();
  r3->set_name("r3");
  //hardware
  r3->set_pin(new GPIOPin(4, OUTPUT, false));
  r3->set_restore_mode(gpio::GPIO_SWITCH_RESTORE_DEFAULT_OFF);
  //register  
  App.register_component(r3);
  App.register_switch(r3);
  mqtt_r3 = new mqtt::MQTTSwitchComponent(r3);
  App.register_component(mqtt_r3);
}

gpio::GPIOSwitch* r4;  //冲洗电磁阀
mqtt::MQTTSwitchComponent *mqtt_r4;

void setup_gpio_switch_r4(){
  r4 = new gpio::GPIOSwitch();
  r4->set_name("r4");
  //hardware
  r4->set_pin(new GPIOPin(25, OUTPUT, false));
  r4->set_restore_mode(gpio::GPIO_SWITCH_RESTORE_DEFAULT_OFF);
  //register
  App.register_component(r4);
  App.register_switch(r4);
  mqtt_r4 = new mqtt::MQTTSwitchComponent(r4);
  App.register_component(mqtt_r4);
}

gpio::GPIOSwitch* r5; //排水电磁阀
mqtt::MQTTSwitchComponent *mqtt_r5;

void setup_gpio_switch_r5(){
  r5 = new gpio::GPIOSwitch();
  r5->set_name("r5");
  //hardware
  r5->set_pin(new GPIOPin(27, OUTPUT, false));
  r5->set_restore_mode(gpio::GPIO_SWITCH_RESTORE_DEFAULT_OFF);
  //register
  App.register_component(r5);
  App.register_switch(r5);
  mqtt_r5 = new mqtt::MQTTSwitchComponent(r5);
  App.register_component(mqtt_r5);
}

gpio::GPIOSwitch* r6;
mqtt::MQTTSwitchComponent *mqtt_r6;

void setup_gpio_switch_r6(){
  r6 = new gpio::GPIOSwitch();
  r6->set_name("r6");
  //hardware
  r6->set_pin(new GPIOPin(14, OUTPUT, false));
  r6->set_restore_mode(gpio::GPIO_SWITCH_RESTORE_DEFAULT_OFF);
  //register
  App.register_component(r6);
  App.register_switch(r6);
  mqtt_r6 = new mqtt::MQTTSwitchComponent(r6);
  App.register_component(mqtt_r6);
}




// void setup4_template(){
//   //template::binary_sensor 
//   leaking = App.make_template_binary_sensor("leaking");
//   leaking->set_template([=]() -> optional<bool> {
//       if (adc_leaking->state < 0.7)
//         return true;
//       else
//         return false;
//   });
// }

// void setup5_display(){
//   //SPI and max7219 display
//   SPIComponent *spicomponent = App.init_spi(5);
//   spicomponent->set_mosi(2);
//   display::MAX7219Component *display_max7219 = App.make_max7219(spicomponent, 15);
//   display_max7219->set_num_chips(1);
//   display_max7219->set_writer([=](display::MAX7219Component & it) {
//       it.print("01234567");    
//   });
// }


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
  mqtt_mqttclientcomponent->set_topic_prefix("water");
  mqtt_mqttclientcomponent->set_birth_message(mqtt::MQTTMessage{
      .topic = "water/status",
      .payload = "online",
      .qos = 0,
      .retain = true,
  });
  mqtt_mqttclientcomponent->set_last_will(mqtt::MQTTMessage{
      .topic = "water/status",
      .payload = "offline",
      .qos = 0,
      .retain = true,
  });
  mqtt_mqttclientcomponent->set_shutdown_message(mqtt::MQTTMessage{
      .topic = "water/status",
      .payload = "offline",
      .qos = 0,
      .retain = true,
  });
  mqtt_mqttclientcomponent->set_log_message_template(mqtt::MQTTMessage{
      .topic = "water/debug",
      .payload = "",
      .qos = 0,
      .retain = true,
  });
  mqtt_mqttclientcomponent->set_keep_alive(15);
  mqtt_mqttclientcomponent->set_reboot_timeout(900000);
}

status_led::StatusLED *status_led_statusled;

void setup_status_led(){
  status_led_statusled = new status_led::StatusLED(new GPIOPin(12, OUTPUT, true));
  App.register_component(status_led_statusled);
  status_led_statusled->pre_setup();
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
  working_mode_command->set_topic("water/working_mode/command");
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

void setup() {
  App.pre_setup("water", __DATE__ ", " __TIME__);
  setup_logger();
  setup_status_led();
  setup_wifi();
  setup_mqtt_broker();
  setup_wifi_signal();
  setup_sensor_uptime();


  setup_gpio_adc_aPressure();
  setup_gpio_adc_bp();
  setup_gpio_adc_leaking();
  setup_gpio_adc_sp();
  setup_gpio_adc_tds();
  setup_gpio_binary_sensor_door();
  setup_gpio_pulse_counter_outlet();
  setup_gpio_pulse_counter_inlet();
  setup_gpio_pulse_counter_source();
  setup_gpio_switch_r1();
  setup_gpio_switch_r2();
  setup_gpio_switch_r3();
  setup_gpio_switch_r4();
  setup_gpio_switch_r5();
  setup_gpio_switch_r6();

  setup_sensor_mqtt_subscriber_working_mode();
  setup_int_sensor_workingstate();

//   setup4_template(); 
  // setup5_display();
  setup_apps_ondebug();
  App.setup();
  automation_setup();
}


void loop() {
  App.loop();
  automation_loop();
}