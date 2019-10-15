
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
    // left.init_io(AdcPin("GPIO_18"),AdcPin("GPIO_34"),PwmPin("GPIO_25"));
    // left.init_pid(1.0f,1.0f,1.0f);
    right.toucher.init(AdcPin("GPIO_33"),0.02061,177.0);
    right.actuator_feedback.init(AdcPin("GPIO_34"), 0.04115, 413.0 -16.0);
    right.actuator_pin = PwmPin("GPIO_27");
    right.actuator_pin.init(5000, 16, 32768);
    right.actuator_pin.start();
    right.pid_controller.Init(2.05, 0, 0.0);
    

    OutputPin __k1_pin("GPIO_13",true);
    OutputPin __k2_pin("GPIO_32",true);
    OutputPin __k3_pin("GPIO_04",true);
    InputPin __button_a("GPIO_12");
    InputPin __button_b("GPIO_23");
}
unsigned long  last_pid_timestamp;
void Weeder::timer_loop(){
    unsigned now = micros();
    uint16_t interval = now- last_pid_timestamp;
    if (interval <=1000*10) return;

    right.pid_loop_with_reading_sensors();
    last_pid_timestamp = now;
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