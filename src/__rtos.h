// #pragma once

// #include "stdint.h"
// #include "freertos/projdefs.h"

// //freertos.h    https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/FreeRTOS.h   

// #define configTICK_RATE_HZ				( ( TickType_t ) 1000 )


// // freertos/timers.h
// typedef void * TimerHandle_t;
// typedef void (*TimerCallbackFunction_t)( TimerHandle_t xTimer );

// void xTimerDelete(TimerHandle_t, uint32_t);


// // portmacro.h   https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/portmacro.h
// 	typedef uint32_t TickType_t;
//     //typedef unsigned long UBaseType_t;
//     #define portBASE_TYPE	int
//     typedef unsigned portBASE_TYPE	UBaseType_t;
// 	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL



// // https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/projdefs.h
// //#define pdMS_TO_TICKS( xTimeInMs ) ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInMs ) * ( TickType_t ) configTICK_RATE_HZ ) / ( TickType_t ) 1000 ) )

// TimerHandle_t xTimerCreate(	const char * const pcTimerName,			/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
//                             const TickType_t xTimerPeriodInTicks,
//                             const UBaseType_t uxAutoReload,
//                             void * const pvTimerID,
//                             TimerCallbackFunction_t pxCallbackFunction );      //?? what is PRIVILEGED_FUNCTION

// // https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/timers.h
// //#define xTimerDelete( xTimer, xTicksToWait ) xTimerGenericCommand( ( xTimer ), tmrCOMMAND_DELETE, 0U, NULL, ( xTicksToWait ) )


// // https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/RTOS/include/timers.h
// //#define xTimerChangePeriod( xTimer, xNewPeriod, xTicksToWait ) xTimerGenericCommand( ( xTimer ), tmrCOMMAND_CHANGE_PERIOD, ( xNewPeriod ), NULL, ( xTicksToWait ) )


// //                                     tasks.h
// void taskENTER_CRITICAL(){}
// void taskEXIT_CRITICAL(){}