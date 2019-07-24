#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
uint32_t __registered_modules_start;
uint32_t __registered_modules_end;

// sdmmc_support.c    https://github.com/Smoothieware/SmoothieV2/blob/7c65922bb1ddc79103430f9747af1c5fa24170ba/Firmware/Hal/sdmmc/src/sdmmc_support.c
bool setup_sdmmc(void){
    return true;
}

