
#include "weeder.h"
#include "HardwareSerial.h"
Weeder* Weeder::__instance = nullptr;
Weeder* Weeder::get_instance(){
    if (__instance == nullptr){
        Serial.println ("Going to create object of Weeder");
        __instance = new Weeder();
    }
    return __instance;
}
Weeder::Weeder(){
}
void Weeder::init(){
    left.init(AdcPin("GPIO_035"),AdcPin("GPIO_34"),PwmPin("GPIO_25"),1.0f,1.0f,1.0f);
    right.init(AdcPin("GPIO_33"),AdcPin("GPIO_18"),PwmPin("GPIO_27"),1.0f,1.0f,1.0f);
    OutputPin __k1_pin("GPIPO_13",true);
    OutputPin __k2_pin("GPIO_32",true);
    OutputPin __k3_pin("GPIO_04",true);
    InputPin __button_a("GPIO_12");
    InputPin __button_b("GPIO_23");
}
void Weeder::timer_loop(){
    left.pid_loop_with_reading_sensors();
    right.pid_loop_with_reading_sensors();
}

#include "FreeRTOS.h"
#include "freertos/timers.h"

static void timer_task(TimerHandle_t handle){
    Weeder::get_instance()->timer_loop ();
}

void weeder_setup(){
    Weeder::get_instance()->init();
    
    int id = 1909;
    int interval_ms = 500;
    TimerHandle_t tmr = xTimerCreate("weeder_timeTask", pdMS_TO_TICKS(interval_ms), pdTRUE, ( void * )id, 
                                    & timer_task);
    if( xTimerStart(tmr, 10 ) != pdPASS ) {
        printf("[E][setup] Timer for Weeder  start error. \n");
    }
    printf("[D][main] Create xTimerTask Weeder is started.\n" );
}