#include "HardwareSerial.h"
#include "_hal/Board/board.h"
#include "main.h"
#include"_SAL/TaskManager.h"


static const char *TAG = "espsmooth.main";

extern float float_value;


void setup(){
    Board::getInstance()->report_memory();
    Board::getInstance()->init();
    Start_Task(ESPHOME);
    Board::getInstance()->report_memory();
    Start_Task(ROBOT);
    delay(5000);   //Keep uartTx empty for ProntFace handshaking.
    Start_TimerTask(CONTROL_ROBOT_MOTORS);
    
    printf("\n\n"); 
    printf("Hi, Mr.ProntFace. You're online now. right?\n ");
    // Board::getInstance()->Board_report_cpu();
    // Board::getInstance()->report_memory();
    // FileHelper::get_instance()->~FileHelper();    //No effection! WHY?
    // Board::getInstance()->report_memory();

    Start_Task(FTP_SERVER);
    Start_Task(TCP_SERVER);
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

