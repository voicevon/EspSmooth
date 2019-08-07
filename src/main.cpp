#include "HardwareSerial.h"
#include "SPIFFS.h"     // ESP class

#include "_hal/board.h"
#include "_hal/stopwatch.h"

#include "main.h"


static const char *TAG = "espsmooth.main";


extern void configureSPIFI();
extern uint32_t SystemCoreClock;
extern void smoothie_startup(void *);


// class ESP  
// https://techtutorialsx.com/2017/12/17/esp32-arduino-getting-the-free-heap/
void show_memory_allocate(){
    printf("--------------------------------------------------------------\n");
    Serial.print("[power on][ESP.getFreeHeap()]      free heap size = ");
    Serial.println(ESP.getFreeHeap());  
    printf("[power on][esp_get_free_heap_size] free heap size = %d \n",esp_get_free_heap_size());   //Are they same? NO! WHY ?
    printf("--------------------------------------------------------------\n");
    delay(40);   //Wait for Serial/printf() is finished processing, seems serial sending is in a another thread/core cpu ?
}

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

// #include "esp32-hal-log.h"
// #include "esp_log.h"
// #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
// void setup_log(){
//     Serial.println(" ================ test log ================ ");

//     esp_log_level_set("*", ESP_LOG_INFO);  
//     ESP_LOGE(TAG, "test1 Error");
//     ESP_LOGW(TAG, "test1 Warning");
//     ESP_LOGI(TAG, "test1 Info");
//     ESP_LOGD(TAG, "test1 Debug");
//     ESP_LOGV(TAG, "test1 Verbose");

//     // esp_log_level_set(TAG,ESP_LOG_VERBOSE);  
//     ESP_LOGE(TAG, "test2 Error");
//     ESP_LOGW(TAG, "test2 Warning");
//     ESP_LOGI(TAG, "test2 Info");
//     ESP_LOGD(TAG, "test2 Debug");
//     ESP_LOGV(TAG, "test2 Verbose");
//     Serial.println(" ================ end of  test log ================ ");
// }


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

    // launch the startup thread which will become the command thread that executes all incoming commands
    // 10000 Bytes stack
    xTaskCreate(smoothie_startup, "CommandThread", 30000, NULL, (tskIDLE_PRIORITY + 2UL), (TaskHandle_t *) NULL);
    
    // vTaskStartScheduler();    Don't call vTaskStartScheduler()     https://esp32.com/viewtopic.php?t=1336
}

extern float float_value;
extern void ControlMotors(TimerHandle_t xTimer);
void setup(){

    show_memory_allocate();
    // setup_spiffs_writting();
    // setup_spiffs_reading();
    setup_smooth(); 

    delay(5000);   //Keep uartTx empty for Printrun handshaking.
    int interval = 1000;
    int id = 1;
    TimerHandle_t tmr = xTimerCreate("ControlMotors", pdMS_TO_TICKS(interval), pdTRUE, ( void * )id, &ControlMotors);
    if( xTimerStart(tmr, 10 ) != pdPASS ) {
        printf("[E][setup] Timer for ControlMotors  start error \n");
    }
    esphome_setup();
}
#include "smoothie/robot/Robot.h"
#include "smoothie/robot/Actuator/DcMotor.h"


uint64_t rtos_report_inteval_second = 5 ;
uint64_t cpu_idle_counter = 0;
uint64_t last_time_stamp = 0;   //us
// Actually, this is the lowest priority task.
void loop(){
    esphome_loop();
    cpu_idle_counter++;

    if(esp_timer_get_time () - last_time_stamp >= rtos_report_inteval_second * 1000000){
        uint16_t passed_time = cpu_idle_counter / 10280 / rtos_report_inteval_second;
        uint16_t uptime_second = esp_timer_get_time() / 1000000;  
        printf("uptime = %i seconds, cpu usage =  %i/%%  ",uptime_second, 100 - passed_time);

        //vTaskList(ptrTaskList);   vTaskList is not supportted?  Jun2019      https://github.com/espressif/esp-idf/issues/416

        cpu_idle_counter = 0;
        last_time_stamp = esp_timer_get_time();

        DcMotor* dc =(DcMotor*) Robot::getInstance()->actuators[2];
        float dc_angle = dc->for_test_read_sensor_position();

        printf("    Y Pos= %f", dc_angle);
        printf("\n");

        
    }

}

