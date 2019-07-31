//Considered just one UART in the whole system. 

#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#include "stddef.h"

typedef void * TaskHandle_t;
#define xTaskHandle TaskHandle_t

size_t read_uart(char * buf, size_t length) ;
size_t write_uart(const char * buf, size_t length);
void set_notification_uart(xTaskHandle h);


#endif