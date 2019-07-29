// RTOS is using hardware Timer0, which is the highest priority hardware timer,
// How to re-asign it hardware Timer1 ? 
// https://esp32.com/viewtopic.php?t=10331


// Esp32 timer 101.  
// https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/

// Source   Priority    Consumer    
// Timer0 , 1 Low       RTOS           
// Timer1,  3 High      step_tick
// Timer2,  2 Middle    StopWatch ? Mightly don't use. 
// Timer3,  Disabled  

#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_

#include "stdint.h"

//RTOS timer        
//https://gist.github.com/shirish47/5b2a04f81eab4281b5c69f678a4da352


// Setup where frequency is in Hz, delay is in microseconds
int stepTicker_setup(uint32_t frequency, uint32_t delay, void *mr0handler, void *mr1handler);

//SmothieV2         
//https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/tmr-setup.c
void stepTicker_stop();
// void tmr0_stop();
void tmr2_stop();

// frequency in HZ
int tmr2_setup(uint32_t frequency, void *timer_handler);



void tmr1_mr1_start();
 int tmr2_set_frequency(uint32_t frequency);

#endif