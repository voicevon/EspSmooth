#include "string.h"
#include "board.h"
#include "_hal/Pin/Pin.h"
#include "_hal/Pin/OutputPin.h"
#include "_hal/Pin/InputPin.h"
#include "_sal/FileHelper.h"

#include "esp32-hal-gpio.h"
#include "Esp.h"
#include "libs/OutputStream.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/ads1115/ads1115.h"


//https://i.ebayimg.com/images/g/j50AAOSwN8FZqBJI/s-l1600.jpg
//This pin is serial0.tx pin.
#define BUILID_IN_LED_PIN  1   

// #include "_sal/FileSys/spiffs_ext.h"
// FileHelper helper = FileHelper();

//  Crashed when using function parameter issue.
//  https://github.com/espressif/arduino-esp32/issues/2092

Board* Board::__instance = nullptr;

Board::Board(){
    if(__instance == nullptr) 
        __instance= this;
}


// configure the board: i2c, spi, s2c, etc...
void Board::__setup_section_bus(ConfigReader cr){
    #define scl_pin_key "scl_pin"
    #define sda_pin_key "sda_pin"
    
    ConfigReader::sub_section_map_t sub_section_bus;
    if(!cr.get_sub_sections("bus", sub_section_bus)) {
        printf("[E][RobotStart][Config][Bus] ERROR:configure-bus: no bus section found\n");
        return;
    }
    auto target_i2c = sub_section_bus.find("i2c_ads1115");
    if(target_i2c == sub_section_bus.end()) {
        printf("[E][RobotStart][setup_section_bus()] can't find i2c_ads1115.xxx\n");
        return; 
    }

    auto& this_i2c = target_i2c->second; // map of ic2 config values for this i2c
    OutputPin ads1115_scl_pin(cr.get_string(this_i2c, scl_pin_key, "nc"));
    InputPin adc1115_sda_pin(cr.get_string(this_i2c, sda_pin_key, "nc"));

    esphome::i2c::I2CComponent* i2c_component = new esphome::i2c::I2CComponent();
    i2c_component->set_scl_pin(ads1115_scl_pin.get_gpio_id());
    i2c_component->set_sda_pin(adc1115_sda_pin.get_gpio_id());
    i2c_component->set_frequency(200000);
    i2c_component->set_scan(false);
    i2c_component->dump_config();   //Doesn't work!
    i2c_component->setup();
    printf("-----I2CComponent\n");
    // ads1115_sensor.set_icon
    esphome::ads1115::ADS1115Component * ads1115_component = new esphome::ads1115::ADS1115Component();
    ads1115_component->set_i2c_parent(i2c_component);
    ads1115_component->set_i2c_address(0x48);
    ads1115_component->set_continuous_mode(true);
    ads1115_component->setup();
    printf("-----ADS1115Component\n");

}

void Board::init(void){
    //load bus drivers
    std::string str = FileHelper::get_instance()->get_file_content("/board.ini",false);
    std::stringstream sss(str);
    ConfigReader cr(sss);
    ConfigReader::section_map_t sm;
    ConfigReader::sub_section_map_t ssmap;
    return;
    __setup_section_bus (cr);

};

void Board::LED_Toggle(uint8_t LEDNumber){

};

void Board::LED_Set(uint8_t LEDNumber, bool On){
    if(LEDNumber == 1){
        pinMode(BUILID_IN_LED_PIN, OUTPUT);
        digitalWrite(BUILID_IN_LED_PIN, !On );
    }
}


void Board::Board_report_cpu(){
    uint64_t chipid = ESP.getEfuseMac();
    uint16_t chip_id[4];
    chip_id[0] = chipid;
    chip_id[1] = chipid>>2;
    chip_id[2] = chipid>>4;
    chip_id[3] = chipid>>6;

    printf(" ------------------------------------ Report Board ------------------------------------ \n");
    printf("     Chip Revision = %i\n",ESP.getChipRevision());
    printf("     CPU Frequency = %i MHz\n", ESP.getCpuFreqMHz());
    printf(" ChipId or MacAddr = %x-%x-%x-%x \n",chip_id[3],chip_id[2],chip_id[1],chip_id[0]);
    // printf("       Cycle Count = %i\n",ESP.getCycleCount());
    printf("       SDK version = %s\n",ESP.getSdkVersion());
    printf("     GPIO assigned = in TODO list. \n");
    printf("\n\n");
}

// class ESP  
// https://techtutorialsx.com/2017/12/17/esp32-arduino-getting-the-free-heap/
void Board::Board_report_memory(){
    printf(" ------------------------------------ Memory report ------------------------------------ \n");
    // Serial.print("[ESP.getFreeHeap()]      free heap size = ");   //Don't use printf here. Why? I don't know.
    // Serial.println(ESP.getFreeHeap());  
    printf("[ESP.getFreeHeap()]      free heap size = %i\n",ESP.getFreeHeap());  //This doesn't work.
    printf("[esp_get_free_heap_size] free heap size = %d \n",esp_get_free_heap_size());   //Are they same? NO! WHY ?
    printf(" \n \n");
    delay(400);   //Wait for Serial/printf() is finished processing, seems serial sending is in a another thread/core cpu ?
}
