#include "timer.h"
#include "esp32-hal-timer.h"
#include "Arduino.h"

// Can one timer handles two types of ISR service?  seems LPC can do that. How about Esp32?
// Should we use hardware TMR3 for unstep?

#define TIMER1 1

void isr_test(){
    Serial.print("*");
}

hw_timer_t* hw_stepTicker;  
// Setup where frequency is in Hz, delay is in microseconds
int stepTicker_setup(uint32_t frequency, uint32_t delay, void *mr0handler, void *mr1handler){
    hw_stepTicker = timerBegin(TIMER1, 80, true);    // interrupto to be generated is edge(true)  or level(false).
    timerAttachInterrupt(hw_stepTicker, (void(*)(void)) mr0handler, true);
    timerAlarmWrite(hw_stepTicker, 1000000/frequency, true);
    
    // timerAttachInterrupt(hw_stepTicker, &isr_test, true);
    // timerAlarmWrite(hw_stepTicker, 100000ULL, true);   
    
    timerAlarmEnable(hw_stepTicker);
    Serial.println("[OK][stepTicker.interrupt] is statred........");
    return 0; 
}
// For unstep.  
void tmr1_mr1_start(){

}


void stepTicker_stop(){
    timerAlarmDisable(hw_stepTicker);
}

void tmr2_stop(){
    
}
// frequency in HZ
int tmr2_setup(uint32_t frequency, void *timer_handler){
    return 0;
}
int tmr2_set_frequency(uint32_t frequency){
    return 0;
}