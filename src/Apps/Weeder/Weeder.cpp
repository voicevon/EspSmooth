

#include "Weeder.h"
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


//  https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
void Weeder::init(){
    right.toucher.init(AdcPin("GPIO_34"),0.0343, 127.78);
    right.actuator_feedback.init(AdcPin("GPIO_33"), 0.0386, 419);
    right.actuator_pin = PwmPin("GPIO_27");
    right.actuator_pin.init(5000, 16, 32768);
    right.actuator_pin.start();
    right.pid_controller.Init(1, 2.05, 0, 0.0);
    right.set_show_debug(false);

    left.toucher.init(AdcPin("GPIO_26"),0.0180, 167.78);
    left.actuator_feedback.init(AdcPin("GPIO_35"), 0.0386, 419);
    left.actuator_pin = PwmPin("GPIO_19");
    left.actuator_pin.init(5000, 16, 32768);
    left.actuator_pin.start();
    left.pid_controller.Init(2, 2.05, 0, 0.0);
    left.set_show_debug(false); 

    OutputPin __k1_pin("GPIO_13",true);
    OutputPin __k2_pin("GPIO_32",true);
    OutputPin __k3_pin("GPIO_04",true);
    InputPin __button_a("GPIO_12");
    InputPin __button_b("GPIO_23");
}
unsigned long  last_pid_timestamp;
void Weeder::timer_loop(){
    unsigned now = micros();
    uint16_t interval = now - last_pid_timestamp;
    if (interval <= 1000 * 10) return;
    delay(10);

    right.pid_loop_with_reading_sensors();
    left.pid_loop_with_reading_sensors();
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