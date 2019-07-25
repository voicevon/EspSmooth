#include "HardwareSerial.h"
#include "esp32-hal-log.h"

#include "_hal/board.h"
#include "_hal/stopwatch.h"

// #include <Arduino_FreeRTOS.h>



static const char *TAG = "espsmooth.main";


extern void configureSPIFI();
extern uint32_t SystemCoreClock;
extern void smoothie_startup(void *);


void setup2()
{
    Serial.begin(115200); 
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
void setup()
{
    Serial.begin(115200);
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
    // xTaskCreate(smoothie_startup, "CommandThread", 10000/4, NULL, (tskIDLE_PRIORITY + 2UL), (TaskHandle_t *) NULL);
    
    // vTaskStartScheduler();    Don't call vTaskStartScheduler()     https://esp32.com/viewtopic.php?t=1336
}


uint64_t cpu_idle_counter = 0;
uint64_t last_time_stamp = 0;   //us
void loop(){
    // Actually, this is the lowest priority task.
    cpu_idle_counter++;

    if(esp_timer_get_time () - last_time_stamp >= 10000000){
        //printf("cpu_idle_counter = %i", cpu_idle_counter);
        uint32_t passed_time = cpu_idle_counter / 10000;
        uint32_t uptime = esp_timer_get_time();
        printf("uptime = %i us, cpu idle counter =  %i\n",uptime, passed_time);

        //vTaskList(ptrTaskList);   vTaskList is not supportted?  Jun2019      https://github.com/espressif/esp-idf/issues/416

        cpu_idle_counter = 0;
        last_time_stamp = esp_timer_get_time();
    }

}

