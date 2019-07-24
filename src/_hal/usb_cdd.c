// usb_cdc  https://github.com/Smoothieware/SmoothieV2/blob/e0ec1fcf1cb69ce4109a8c20c0212a1bc43004c3/Firmware/Hal/usb_cdc/src/cdc_main.c

#include <stddef.h>

typedef void * TaskHandle_t;
#define xTaskHandle TaskHandle_t


size_t write_cdc(const char *buf, size_t len){size_t xx; xx=1; return xx;};
size_t read_cdc(char *buf, size_t len){size_t xx; xx=1; return xx;};
int setup_cdc(xTaskHandle h){return 0;};

