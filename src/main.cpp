#include "HardwareSerial.h"
#include "_hal/Board/board.h"
#include "main.h"
#include"_SAL/TaskManager.h"


static const char *TAG = "espsmooth.main";
// work as a mqtt sensor.
int16_t current_working_state;


extern float float_value;
//test stepper motor driver.
void setup2(){
    pinMode(16,OUTPUT);
    pinMode(17,OUTPUT);
    pinMode(21,OUTPUT);

    digitalWrite(21,LOW);
    digitalWrite(17,LOW);
    int interval = 200;
    while(1){
        digitalWrite(16,HIGH);
        delayMicroseconds(10);
        digitalWrite(16,LOW);
        delayMicroseconds(interval);

    }
}
void setup(){
    Board::getInstance()->report_memory();
    Board::getInstance()->init();
    Start_Task(ESPHOME);
    Board::getInstance()->report_memory();
    printf("\n\n\n\n"); 
    printf("------------ %i",xPortGetCoreID());
    Start_Task(ROBOT);
    printf("\n\n\n\n"); 
    // Board::getInstance()->Board_report_cpu();
    // Board::getInstance()->report_memory();
    // FileHelper::get_instance()->~FileHelper();    //No effection! WHY?
    // Board::getInstance()->report_memory();
    // Start_Task(FTP_SERVER);
    // printf("\n\n\n");
    // Start_Task(TCP_SERVER);
    // printf("\n\n\n");
    // Start_TimerTask(CONTROL_ROBOT_MOTORS);
    printf("setup() is completed! \n\n\n");
    printf("Hi, Mr.ProntFace. You're online now. right?\n ");

    // printf("Connected!\n");
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

        cpu_idle_counter = 0;
        last_time_stamp = esp_timer_get_time();

        // DcMotor* dc =(DcMotor*) Robot::getInstance()->actuators[1];
        // // float dc_angle = dc->for_test_read_sensor_position();
        // float dc_angle= dc->get_current_position();
        // printf("    Y Pos= %f", dc_angle);
        printf("\n");
    }

}

