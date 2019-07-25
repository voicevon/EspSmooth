#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#include "stddef.h"

typedef void * TaskHandle_t;
#define xTaskHandle TaskHandle_t

//uart  //https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/uart_comms.h
// int setup_uart();
size_t read_uart(char * buf, size_t length) ;
size_t write_uart(const char * buf, size_t length);
void set_notification_uart(xTaskHandle h);


#endif