#pragma once
#ifndef _HAL_BOARD_H_
#define _HAL_BOARD_H_

#include "stdint.h"
// board  https://github.com/Smoothieware/SmoothieV2/blob/ed7237065621f55f37fd420ea0b78b91bc411d63/Firmware/Hal/lpc_board/src/board.c
void Board_Init(void);
void Board_LED_Toggle(uint8_t LEDNumber);
void Board_LED_Set(uint8_t LEDNumber, bool On);

// cpu    https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/Hal/lpc_chip_43xx/inc/core_cm3.h
void NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
//    cpu https://github.com/Smoothieware/SmoothieV2/blob/7c65922bb1ddc79103430f9747af1c5fa24170ba/flashloader/lpc_chip_43xx/src/chip_18xx_43xx.c
extern uint32_t SystemCoreClock;

void SystemCoreClockUpdate(void);
//    cpu https://github.com/Smoothieware/SmoothieV2/blob/7c65922bb1ddc79103430f9747af1c5fa24170ba/flashloader/lpc_chip_43xx/src/chip_18xx_43xx.c


#endif