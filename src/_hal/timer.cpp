#include "timer.h"

void tmr1_stop(){

}

void tmr0_stop(){
    
}
// frequency in HZ
int tmr1_setup(uint32_t frequency, void *timer_handler){
    return 0;
}
// Setup where frequency is in Hz, delay is in microseconds
int tmr0_setup(uint32_t frequency, uint32_t delay, void *mr0handler, void *mr1handler){
    return 0;
}

void tmr0_mr1_start(){

}
int tmr1_set_frequency(uint32_t frequency){
    return 0;
}