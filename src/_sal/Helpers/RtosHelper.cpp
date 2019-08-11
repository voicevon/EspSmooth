#include "RtosHelper.h"


RtosHelper* RtosHelper::__instance = nullptr;

RtosHelper::RtosHelper(){
    if(__instance == nullptr) __instance = this;
}

void RtosHelper::add(TaskHandle_t* task_handle){
    __task_list[__next_id] = task_handle;
    __next_id++;
}


//  uxTaskGetSystemState() working BUT runtime stats vary wildly
//  https://esp32.com/viewtopic.php?f=2&t=1532

//  path:: freeRtos/freeRtos/freertosConfig.h
//  find:  CONFIG_FREERTOS_USE_TRACE_FACILITY
//  comment // #ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
//          and #endif
//  enter:  .pio
//  delete: 
// #define configUSE_TRACE_FACILITY        1       /* Used by uxTaskGetSystemState(), and other trace facility functions */
// #endif

// C:/Users/xm/.platformio>package/framework-arduinoespressif32/tools/sdk/sdkconfig
// Search:  CONFIG_FREERTOS_USE_TRACE_FACILITY=
// add 'y',  It doesn't recompile.
void RtosHelper::print_task_list(){
    printf ("---------------------- RtosHelper tasks -----------------------\n");
    printf(" Task name      Priority    Status       cpu    stack   \n");
    TaskHandle_t th;
    for(int i=0;i<__next_id;i++){
        eTaskState xx = eTaskGetState(__task_list[i]);
        TaskStatus_t* ts ;
        UBaseType_t uxSize;
        uint32_t total_time;
        // uxTaskGetSystemState(ts,uxSize,&total_time);

        printf("%s,%s,\n",ts->pcTaskName);
    }
}
