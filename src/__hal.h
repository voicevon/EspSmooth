#pragma once
//  Is this our target?
//  https://github.com/espressif/arduino-esp32/blob/fa74767b2e4b4808439f81b97cb454e1118621f8/cores/esp32/esp32-hal.h

#include "libs/HAL/Pin.h"
#include "libs/HAL/Pwm.h"
#include "libs/HAL/Adc.h"


#define BUILD_TARGET "ESP32_WROOM"


bool isnan(float xx){    return false;}




// https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/tmr-setup.c
void tmr1_stop(){}

// frequency in HZ
int tmr1_setup(uint32_t frequency, void *timer_handler){ return 1;}

int tmr1_set_frequency(uint32_t frequency)  {return 1;}

typedef void * TaskHandle_t;
#define xTaskHandle TaskHandle_t
//uart  //https://github.com/Smoothieware/SmoothieV2/blob/97f5278ae5f26b751c71a98927c8c539ff78a2e8/Firmware/Hal/src/uart_comms.h
int setup_uart(){return 1;}
size_t read_uart(char * buf, size_t length);
size_t write_uart(const char * buf, size_t length){return 1;}
void set_notification_uart(xTaskHandle h);


// board  https://github.com/Smoothieware/SmoothieV2/blob/ed7237065621f55f37fd420ea0b78b91bc411d63/Firmware/Hal/lpc_board/src/board.c
void Board_Init(void);
void Board_LED_Toggle(uint8_t LEDNumber);
void Board_LED_Set(uint8_t LEDNumber, bool On);

// cpu    https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/Hal/lpc_chip_43xx/inc/core_cm3.h
void NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
//    cpu https://github.com/Smoothieware/SmoothieV2/blob/7c65922bb1ddc79103430f9747af1c5fa24170ba/flashloader/lpc_chip_43xx/src/chip_18xx_43xx.c
uint32_t SystemCoreClock;
void SystemCoreClockUpdate(void);

// stopwatch  https://github.com/Smoothieware/SmoothieV2/blob/46fa7b9afa093e504639172c06b06aff42bd0f36/Firmware/Hal/lpc_chip_43xx/inc/stopwatch.h
void StopWatch_Init(void);
uint32_t StopWatch_TicksPerSecond(void){return 1;}
