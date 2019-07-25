#include "board.h"
#include "Arduino.h"

//    cpu https://github.com/Smoothieware/SmoothieV2/blob/7c65922bb1ddc79103430f9747af1c5fa24170ba/flashloader/lpc_chip_43xx/src/chip_18xx_43xx.c
uint32_t SystemCoreClock;  // System Clock Frequency (Core Clock) 


//https://i.ebayimg.com/images/g/j50AAOSwN8FZqBJI/s-l1600.jpg
//This pin is serial0.tx pin.
#define BUILID_IN_LED_PIN  1   

void Board_Init(void){
    Serial.begin(115200);
};
void Board_LED_Toggle(uint8_t LEDNumber){};
void Board_LED_Set(uint8_t LEDNumber, bool On){
    if(LEDNumber == 1){
        pinMode(BUILID_IN_LED_PIN,OUTPUT);
        digitalWrite(BUILID_IN_LED_PIN, !On );
    }
}

void SystemCoreClockUpdate(void){
    SystemCoreClock = 1;   //10Khz for RTOS time_slot.   240Mhz?  
}