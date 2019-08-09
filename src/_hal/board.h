#pragma once
#ifndef _HAL_BOARD_H_
#define _HAL_BOARD_H_

#include "stdint.h"
#include "stdbool.h"


//TODO:  SingleTon
class Board
{
    public:
        static Board *getInstance() { return __instance; }
        Board();
        // delete copy and move constructors and assign operators
        Board(Board const&) = delete;             // Copy construct
        Board(Board&&) = delete;                  // Move construct
        Board& operator=(Board const&) = delete;  // Copy assign
        Board& operator=(Board &&) = delete;      // Move assign

        void init(void);
        void Board_LED_Toggle(uint8_t LEDNumber);
        void Board_LED_Set(uint8_t LEDNumber, bool On);
        void Board_report_cpu();
        void Board_report_memory();
    
    private:
        static Board* __instance;
};

#endif