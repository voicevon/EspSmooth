
#include "weeder.h"

Weeder* Weeder::__instance = nullptr;

Weeder::Weeder(){

}
void Weeder::init(){
    left.init(AdcPin("GPIO_01"),AdcPin("GPIO_02"),PwmPin("GPIO_03"),1.0f,1.0f,1.0f);
    right.init(AdcPin("GPIO_01"),AdcPin("GPIO_02"),PwmPin("GPIO_03"),1.0f,1.0f,1.0f);

}
void Weeder::timer_loop(){
    left.pid_loop_with_reading_sensors();
    right.pid_loop_with_reading_sensors();
}

#include "FreeRTOS.h"
#include "freertos/timers.h"
void weeder_setup(){
    Weeder::get_instance()->init();
    // SlowTicker::getInstance()->attach(interval_ms, &test_slowStick);
    
    int id = 1909;
    int interval_ms = 500;
    // TimerHandle_t tmr = xTimerCreate("robot_motors_movement", pdMS_TO_TICKS(interval_ms), pdTRUE, ( void * )id, 
    //                                 & Weeder::get_instance()->timer_loop);
    // if( xTimerStart(tmr, 10 ) != pdPASS ) {
    //     printf("[E][setup] Timer for ControlMotors  start error. \n");
    // }
    // printf("[D][main] Create xTimerTask COntrolMotors is started.\n" );
}