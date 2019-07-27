// RTOS is using hardware Timer0, which is the highest priority hardware timer,
// How to re-asign it hardware Timer1 ? 
// https://esp32.com/viewtopic.php?t=10331


// Timer0 , for stepper motor
// Timer1,  for 

#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_

#include "stdint.h"

//RTOS timer        
//https://gist.github.com/shirish47/5b2a04f81eab4281b5c69f678a4da352

//SmothieV2         
//https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/tmr-setup.c
void tmr1_stop();
void tmr0_stop();

// frequency in HZ
int tmr1_setup(uint32_t frequency, void *timer_handler);

// Setup where frequency is in Hz, delay is in microseconds
int tmr0_setup(uint32_t frequency, uint32_t delay, void *mr0handler, void *mr1handler);

void tmr0_mr1_start();
int tmr1_set_frequency(uint32_t frequency);

#endif