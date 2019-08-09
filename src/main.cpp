#include "HardwareSerial.h"
#include "SPIFFS.h"     // ESP class

#include "_hal/Board/board.h"
#include "_hal/stopwatch.h"
#include "smoothie/robot/Actuator/ActuatorTask.h"
#include "smoothie/RobotStarter.h"
#include "main.h"


static const char *TAG = "espsmooth.main";


// extern void configureSPIFI();
// extern void smoothie_startup(void *);


// void smooth_setup(){

//     StopWatch_Init();
//     printf("StopWatch clock rate= %lu Hz\n", StopWatch_TicksPerSecond());

//     // launch the startup thread which will become the command thread that executes all incoming commands
//     // 10000 Bytes stack
//     xTaskCreate(smoothie_startup, "CommandThread", 30000, NULL, (tskIDLE_PRIORITY + 2UL), (TaskHandle_t *) NULL);
    
// }

extern float float_value;
#include <esp_log.h>
#include "_sal/FileHelper.h"
std::string test(const char* cc){
    printf("%s\n",cc);
    std::string xx = "aaaaaaa";
    return xx;
}
void setup(){
    // esp_log_level_set("*", ESP_LOG_DEBUG);
    // Serial.begin(115200);
    Board::getInstance()->Board_report_memory();
    Board::getInstance()->init();
    esphome_setup();   //wifi setup must be in advance of starting a timer_interrupt. Even RTOS. 
    // return;
    Board::getInstance()->Board_report_memory();
    // return;
    smoothie_setup(); 
    delay(5000);   //Keep uartTx empty for ProntFace handshaking.
    printf("\n\n"); 
    printf("Hi, Mr.ProntFace. You're online now. right?\n ");
    // return;
    Controlmotors_setup();
    Board::getInstance()->Board_report_cpu();
    Board::getInstance()->Board_report_memory();
    FileHelper::get_instance()->~FileHelper();    //No effection! WHY?
    Board::getInstance()->Board_report_memory();
}

#include "smoothie/robot/Robot.h"
#include "smoothie/robot/Actuator/DcMotor.h"

uint64_t rtos_report_inteval_second = 5 ;
uint64_t cpu_idle_counter = 0;
uint64_t last_time_stamp = 0;   //us
// Actually, this is the lowest priority task.
void loop(){
    cpu_idle_counter++;
    if(esp_timer_get_time () - last_time_stamp >= rtos_report_inteval_second * 1000000){
        uint16_t passed_time = cpu_idle_counter / 10280 / rtos_report_inteval_second;
        uint16_t uptime_second = esp_timer_get_time() / 1000000;  
        printf("uptime = %i seconds, cpu usage =  %i/%%  ",uptime_second, 100 - passed_time);

        //vTaskList(ptrTaskList);   vTaskList is not supportted?  Jun2019      https://github.com/espressif/esp-idf/issues/416

        cpu_idle_counter = 0;
        last_time_stamp = esp_timer_get_time();

        // DcMotor* dc =(DcMotor*) Robot::getInstance()->actuators[1];
        // // float dc_angle = dc->for_test_read_sensor_position();
        // float dc_angle= dc->get_current_position();
        // printf("    Y Pos= %f", dc_angle);
        printf("\n");
        
    }

}

