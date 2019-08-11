#pragma once
#include "FreeRTOS.h"
#include "freertos/task.h"

class RtosHelper{

    public:
        static RtosHelper* get_instance(){ return __instance; }
        void add(TaskHandle_t* task_handle);
        void report_task_list();
    private:
        RtosHelper();
        static RtosHelper* __instance;
        TaskHandle_t __task_list[10];
        int __next_id;
};

//   Task states diagram
//   https://slideplayer.com/slide/8645974/26/images/3/Task+States+Diagram.jpg