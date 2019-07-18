#pragma once


#include "libs/HAL/Pin.h"
#include "libs/HAL/Pwm.h"
#include "libs/HAL/Adc.h"

bool isnan(float xx){    return false;}




// https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/tmr-setup.c
void tmr1_stop(){}

// frequency in HZ
int tmr1_setup(uint32_t frequency, void *timer_handler){ return 1;}

int tmr1_set_frequency(uint32_t frequency)  {return 1;}

typedef void * TaskHandle_t;
#define xTaskHandle TaskHandle_t
//uart  //https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/uart_comms.h
void set_notification_uart(xTaskHandle h){}

size_t read_uart(char * buf, size_t length){};
size_t write_uart(const char * buf, size_t length){}


// board  https://github.com/Smoothieware/SmoothieV2/blob/ed7237065621f55f37fd420ea0b78b91bc411d63/Firmware/Hal/lpc_board/src/board.c
void Board_LED_Toggle(uint8_t LEDNumber){}


