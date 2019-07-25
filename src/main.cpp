#include "HardwareSerial.h"
#include "esp32-hal-log.h"

#include "_hal/board.h"
#include "_hal/stopwatch.h"

// #include "FreeRTOS.h"
// #include "freertos/task.h"



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
    Board_LED_Set(3, true);

    //ESP_LOGE(TAG,"setup completed, RTOS is begining ........................");

    // launch the startup thread which will become the command thread that executes all incoming commands
    // 10000 Bytes stack
    xTaskCreate(smoothie_startup, "CommandThread", 10000/4, NULL, (tskIDLE_PRIORITY + 2UL), (TaskHandle_t *) NULL);



    /* Start the scheduler */
    vTaskStartScheduler();

    // never gets here
    return;
}



void loop(){
    // esphome::App.loop();
    // ESP_LOGD(TAG,"Main.loop()");
       ESP_LOGE(TAG,"RTOS is not started !!! ");
      delay(1000);

}