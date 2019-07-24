#include "rtos.h"

void __taskENTER_CRITICAL(){}
void __taskEXIT_CRITICAL(){}


#include "FreeRTOS.h"
#include "freertos/task.h"

UBaseType_t uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t * const pulTotalRunTime ){
    return 1;
}
