#pragma once

#include "FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

void set_notification_tcp(xTaskHandle h);
int setup_tcp(uint32_t baudrate);
void stop_tcp();
size_t read_tcp(char * buf, size_t length);
size_t write_tcp(const char * buf, size_t length);

#ifdef __cplusplus
}
#endif