#include "HardwareSerial.h"
#include "esp32-hal-log.h"
#include "SPIFFS.h"

#include "_hal/board.h"
#include "_hal/stopwatch.h"

// #include <Arduino_FreeRTOS.h>



static const char *TAG = "espsmooth.main";


extern void configureSPIFI();
extern uint32_t SystemCoreClock;
extern void smoothie_startup(void *);

void setup_spiffs_writting() {
    if(!SPIFFS.begin(true)) {
        Serial.println ("An eooro has occurred while mounting SPIFFS");
        return;
    }

    File file =  SPIFFS.open("/config.ini",FILE_WRITE);
    if(!file) {
        Serial.println("There was an error opening the file for wrtting");
        return;
    }

    if(!file.print("TEST")) {
        Serial.println("File write failed");
    }

    file.close();

}
void setup_spiffs_reading(){
    if(!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS ");
        return;
    }

    File file = SPIFFS.open ("/config.ini",FILE_READ);
    if(!file) {
        Serial.println("There was an error opening the file for reading");
        return;
    }

    Serial.println("File content:");
    while(file.available()) {
        Serial.write (file.read());
        //Serial.println();
    }

    file.close();
}

void setup_log(){
    Serial.println("==============================================");

    esp_log_level_set("*", ESP_LOG_INFO);  
    ESP_LOGE("TAG", "1Error");
    ESP_LOGW("TAG", "1Warning");
    ESP_LOGI("TAG", "1Info");
    ESP_LOGD("TAG", "1Debug");
    ESP_LOGV("TAG", "1Verbose");

    esp_log_level_set(TAG,ESP_LOG_VERBOSE);  
    ESP_LOGE("TAG", "Error");
    ESP_LOGW("TAG", "Warning");
    ESP_LOGI("TAG", "Info");
    ESP_LOGD("TAG", "Debug");
    ESP_LOGV("TAG", "Verbose");
}

void setup_smooth(){
    NVIC_SetPriorityGrouping( 0 );
    SystemCoreClockUpdate();

    Board_Init();

#ifndef FLASH16BIT
    configureSPIFI(); // setup the winbond SPIFI to max speed
#endif

    printf("MCU clock rate= %lu Hz\n", SystemCoreClock);

    StopWatch_Init();
    printf("StopWatch clock rate= %lu Hz\n", StopWatch_TicksPerSecond());



    // led 4 indicates boot phase 1 complete
    // Board_LED_Set(3, true);


    // launch the startup thread which will become the command thread that executes all incoming commands
    // 10000 Bytes stack
    xTaskCreate(smoothie_startup, "CommandThread", 30000, NULL, (tskIDLE_PRIORITY + 2UL), (TaskHandle_t *) NULL);
    
    // vTaskStartScheduler();    Don't call vTaskStartScheduler()     https://esp32.com/viewtopic.php?t=1336
}


void task2_test(void*){
    for(;;){
        delay(1000);
        printf("task 2  is live....\n");
    }
}
void task1_test(void*){
    xTaskCreate(task2_test, "test_task 2", 30000, NULL, (tskIDLE_PRIORITY + 2UL), (TaskHandle_t *) NULL);

    for(;;){
        delay(1000);
        printf("task 1  is live....\n");
    }
}

void setup_test()
{
    xTaskCreate(task1_test, "test_task 1", 30000, NULL, (tskIDLE_PRIORITY + 2UL), (TaskHandle_t *) NULL);

}
void setup(){
    Serial.begin(115200);
    //setup_smooth();
    //setup_log();
    //setup_spiffs_writting();
    //setup_spiffs_reading();
    setup_test();
}

uint64_t cpu_idle_counter = 0;
uint64_t last_time_stamp = 0;   //us
void loop(){
    // Actually, this is the lowest priority task.
    cpu_idle_counter++;

    if(esp_timer_get_time () - last_time_stamp >= 10000000){
        //printf("cpu_idle_counter = %i", cpu_idle_counter);
        uint16_t passed_time = cpu_idle_counter / 10000;
        uint16_t uptime_second = esp_timer_get_time() / 1000000;  
        printf("uptime = %i seconds, cpu idle counter =  %i\n",uptime_second, passed_time);

        //vTaskList(ptrTaskList);   vTaskList is not supportted?  Jun2019      https://github.com/espressif/esp-idf/issues/416

        cpu_idle_counter = 0;
        last_time_stamp = esp_timer_get_time();
    }

}

