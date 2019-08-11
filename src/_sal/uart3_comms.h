#pragma once

#include "FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

void set_notification_uart3(xTaskHandle h);
int setup_uart3(uint32_t baudrate);
void stop_uart3();
size_t read_uart3(char * buf, size_t length);
size_t write_uart3(const char * buf, size_t length);

#ifdef __cplusplus
}
#endif