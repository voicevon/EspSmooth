#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_

#include "stdint.h"

// https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/tmr-setup.c
void tmr1_stop(){}
void tmr0_stop(){}

// frequency in HZ
int tmr1_setup(uint32_t frequency, void *timer_handler){ return 1;}
// Setup where frequency is in Hz, delay is in microseconds
int tmr0_setup(uint32_t frequency, uint32_t delay, void *mr0handler, void *mr1handler){return 0;};

void tmr0_mr1_start(){};
int tmr1_set_frequency(uint32_t frequency)  {return 1;}

#endif