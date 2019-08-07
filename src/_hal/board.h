#pragma once
#ifndef _HAL_BOARD_H_
#define _HAL_BOARD_H_

#include "stdint.h"
#include "stdbool.h"


// board  https://github.com/Smoothieware/SmoothieV2/blob/ed7237065621f55f37fd420ea0b78b91bc411d63/Firmware/Hal/lpc_board/src/board.c
void Board_Init(void);
void Board_LED_Toggle(uint8_t LEDNumber);
void Board_LED_Set(uint8_t LEDNumber, bool On);
void Board_report_cpu();
void Board_report_memory();


#endif