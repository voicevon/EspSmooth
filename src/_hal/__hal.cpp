#include "__hal.h"



bool isnan(float xx){
    return false;
}


// cpu    https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/Hal/lpc_chip_43xx/inc/core_cm3.h
void NVIC_SetPriorityGrouping(uint32_t PriorityGroup){

}




// stopwatch  https://github.com/Smoothieware/SmoothieV2/blob/46fa7b9afa093e504639172c06b06aff42bd0f36/Firmware/Hal/lpc_chip_43xx/inc/stopwatch.h
// void StopWatch_Init(void){};
// uint32_t StopWatch_TicksPerSecond(void){return 1;}

// unity_internals.h  https://github.com/Smoothieware/SmoothieV2/blob/fc46e2b88e431686f5a4f83e04e2e2c58fa20692/Firmware/Unity/src/unity_internals.h
// #define isinf(n) (isnan((n) - (n)) && !isnan(n))

void __asm_ (const char* xxxx){

}

// mem.c   https://github.com/Smoothieware/SmoothieV2/blob/9d6a67df2b3d119a85fec76696850bd699616bb2/Firmware/Hal/network/lwip/core/mem.c
typedef size_t mem_size_t;
void * mem_malloc(mem_size_t size){
    void * xx;
    return xx;
}

void mem_free(void *rmem){

}
