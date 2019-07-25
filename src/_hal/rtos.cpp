#include "rtos.h"

void __taskENTER_CRITICAL(){}
void __taskEXIT_CRITICAL(){}


#include "FreeRTOS.h"
#include "freertos/task.h"

UBaseType_t uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t * const pulTotalRunTime ){
    return 1;
}



#include "_hal/board.h"
#include "_hal/__hal.h"
#include "_hal/uart.h"
// hooks from freeRTOS
// extern "C" void vApplicationIdleHook( void )
// {
//     static TickType_t last_time_check = xTaskGetTickCount();
//     if(TICKS2MS(xTaskGetTickCount() - last_time_check) >= 300) {
//         last_time_check = xTaskGetTickCount();
//         if(!config_error_msg.empty()) {
//             // handle config error
//             // flash both leds
//             Board_LED_Toggle(0);
//             Board_LED_Toggle(1);
//         } else {
//             // handle play led 1 and aux play led
//             if(system_running) {
//                 if(Module::is_halted()) {
//                     Board_LED_Toggle(1);
//                     if(aux_play_led != nullptr) {
//                         aux_play_led->set(!aux_play_led->get());
//                     }
//                 }else{
//                     Board_LED_Set(1, !Conveyor::getInstance()->is_idle());
//                     if(aux_play_led != nullptr) {
//                         aux_play_led->set(!Conveyor::getInstance()->is_idle());
//                     }
//                 }
//             }
//         }
//     }
// }

// extern "C" void vApplicationTickHook( void )
// {
//     /* This function will be called by each tick interrupt if
//     configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
//     added here, but the tick hook is called from an interrupt context, so
//     code must not attempt to block, and only the interrupt safe FreeRTOS API
//     functions can be used (those that end in FromISR()). */
// }

extern "C" void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    Board_LED_Set(0, true);
    Board_LED_Set(1, false);
    Board_LED_Set(2, true);
    Board_LED_Set(3, true);
   __asm_("bkpt #0");
    for( ;; );
}


extern "C" void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    #if 0
    taskDISABLE_INTERRUPTS();
    __asm_("bkpt #0");
    for( ;; );
    #else
    // we don't want to use any memory for this
    // returns NULL to the caller
    write_uart("FATAL: malloc/sbrk out of memory\n", 33);
    return;
    #endif
}

extern "C" void HardFault_Handler(void) {
    Board_LED_Set(0, true);
    Board_LED_Set(1, true);
    Board_LED_Set(2, false);
    Board_LED_Set(3, false);
    __asm_("bkpt #0");
    for( ;; );
}