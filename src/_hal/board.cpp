#include "board.h"


//    cpu https://github.com/Smoothieware/SmoothieV2/blob/7c65922bb1ddc79103430f9747af1c5fa24170ba/flashloader/lpc_chip_43xx/src/chip_18xx_43xx.c
uint32_t SystemCoreClock;


void Board_Init(void){};
void Board_LED_Toggle(uint8_t LEDNumber){};
void Board_LED_Set(uint8_t LEDNumber, bool On){};

void SystemCoreClockUpdate(void){
    SystemCoreClock = 1;   //10Khz for RTOS time_slot.   240Mhz?  
}