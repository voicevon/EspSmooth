#include "string.h"
#include "board.h"
#include "_sal/FileHelper.h"
#include "_sal/configure/ConfigReader.h"
#include "esp32-hal-gpio.h"
#include "Esp.h"
#include "libs/OutputStream.h"

//https://i.ebayimg.com/images/g/j50AAOSwN8FZqBJI/s-l1600.jpg
//This pin is serial0.tx pin.
#define BUILID_IN_LED_PIN  1   

#include "_sal/FileSys/spiffs_ext.h"
FileHelper helper = FileHelper();

//  Crashed when using function parameter issue.
//  https://github.com/espressif/arduino-esp32/issues/2092
void Board_Init(void){
    //load bus drivers
    const char*  file_name = "/board.ini";
    std::string str = helper.get_file_content("/board.ini",true);
    std::stringstream sss(str);
};

void Board_LED_Toggle(uint8_t LEDNumber){

};

void Board_LED_Set(uint8_t LEDNumber, bool On){
    if(LEDNumber == 1){
        pinMode(BUILID_IN_LED_PIN, OUTPUT);
        digitalWrite(BUILID_IN_LED_PIN, !On );
    }
}


void Board_report_cpu(){
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
void Board_report_memory(){
    printf(" ------------------------------------ Memory report ------------------------------------ \n");
    // Serial.print("[ESP.getFreeHeap()]      free heap size = ");   //Don't use printf here. Why? I don't know.
    // Serial.println(ESP.getFreeHeap());  
    printf("[ESP.getFreeHeap()]      free heap size = %i\n",ESP.getFreeHeap());  //This doesn't work.
    printf("[esp_get_free_heap_size] free heap size = %d \n",esp_get_free_heap_size());   //Are they same? NO! WHY ?
    printf(" \n \n");
    delay(400);   //Wait for Serial/printf() is finished processing, seems serial sending is in a another thread/core cpu ?
}
