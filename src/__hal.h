#pragma once


#include "libs/HAL/Pin/Pin.h"

bool isnan(float xx){
    return false;
}

// freertos/timers.h
typedef void * TimerHandle_t;



// https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/tmr-setup.c
void tmr1_stop(){}

// frequency in HZ
int tmr1_setup(uint32_t frequency, void *timer_handler){
    return 1;
}

int tmr1_set_frequency(uint32_t frequency){
    return 1;
}

#include "FreeRTOS.h"
#include "freertos/timers.h"
// https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/timers.h
#define xTimerDelete( xTimer, xTicksToWait ) xTimerGenericCommand( ( xTimer ), tmrCOMMAND_DELETE, 0U, NULL, ( xTicksToWait ) )


// https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/timers.h
#define xTimerChangePeriod( xTimer, xNewPeriod, xTicksToWait ) xTimerGenericCommand( ( xTimer ), tmrCOMMAND_CHANGE_PERIOD, ( xNewPeriod ), NULL, ( xTicksToWait ) )