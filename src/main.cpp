#include "HardwareSerial.h"
#include "SPIFFS.h"     // ESP class

#include "_hal/board.h"
#include "_hal/stopwatch.h"

#include "main.h"


static const char *TAG = "espsmooth.main";


extern void configureSPIFI();
extern void smoothie_startup(void *);


void smooth_setup(){

#ifndef FLASH16BIT
    configureSPIFI(); // setup the winbond SPIFI to max speed
#endif
    StopWatch_Init();
    printf("StopWatch clock rate= %lu Hz\n", StopWatch_TicksPerSecond());

    // launch the startup thread which will become the command thread that executes all incoming commands
    // 10000 Bytes stack
    xTaskCreate(smoothie_startup, "CommandThread", 30000, NULL, (tskIDLE_PRIORITY + 2UL), (TaskHandle_t *) NULL);
    
    // vTaskStartScheduler();    Don't call vTaskStartScheduler()     https://esp32.com/viewtopic.php?t=1336
}

extern float float_value;
extern void ControlMotors(TimerHandle_t xTimer);   //@ smoothis/robot/Actuator/ActuatorTask.cpp
void control_motor_setup(){
    int interval = 1000;
    int id = 1;
    TimerHandle_t tmr = xTimerCreate("ControlMotors", pdMS_TO_TICKS(interval), pdTRUE, ( void * )id, &ControlMotors);
    if( xTimerStart(tmr, 10 ) != pdPASS ) {
        printf("[E][setup] Timer for ControlMotors  start error. \n");
    }
    printf("[D][main] Create xTimerTask COntrolMotors is started.\n" );
}

void setup(){
    Board_report_cpu();
    Board_report_memory();
    Board_Init();
    esphome_setup();   //wifi setup must be in advance of starting a timer_interrupt. Even RTOS. 
    Board_report_memory();

    smooth_setup(); 
    delay(5000);   //Keep uartTx empty for ProntFace handshaking.
    printf("\n\n"); 
    printf("Hi, Mr.ProntFace. You're online now. right?\n ");
    control_motor_setup();
    Board_report_memory();
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

        DcMotor* dc =(DcMotor*) Robot::getInstance()->actuators[1];
        // float dc_angle = dc->for_test_read_sensor_position();
        float dc_angle= dc->get_current_position();
        printf("    Y Pos= %f", dc_angle);
        printf("\n");
        
    }

}

