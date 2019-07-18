#pragma once

#include "stdint.h"


void __taskENTER_CRITICAL();
void __taskEXIT_CRITICAL();



// freertos/timers.h
// typedef void * TimerHandle_t;
// void xTimerDelete(TimerHandle_t, uint32_t);

// https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/timers.h
//#define xTimerDelete( xTimer, xTicksToWait ) xTimerGenericCommand( ( xTimer ), tmrCOMMAND_DELETE, 0U, NULL, ( xTicksToWait ) )


// https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/timers.h
//#define xTimerChangePeriod( xTimer, xNewPeriod, xTicksToWait ) xTimerGenericCommand( ( xTimer ), tmrCOMMAND_CHANGE_PERIOD, ( xNewPeriod ), NULL, ( xTicksToWait ) )


//                                     tasks.h
// void taskENTER_CRITICAL(){}
// void taskEXIT_CRITICAL(){}