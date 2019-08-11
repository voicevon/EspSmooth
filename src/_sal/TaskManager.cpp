#include "TaskManager.h"
#include "_SAL/FtpServer/ESP32FtpServer.h"


FtpServer* __fpt;
void ftp_loop(void*){
    while(true){
        __fpt->handleFTP();
    }
}

void start_ftp(){
    //read config
    __fpt = new FtpServer();
    __fpt->begin("a","a");
    xTaskCreate(ftp_loop, "ftp_loop", 30000, NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);

}
void Start_Task(TASK_ITEMS_T target_task){
    switch (target_task)
    {
    case FTP_SERVER:
        start_ftp();
        break;
    case SERIAL_COMM:

        break;
    case TCP_COMM:
        break;
    case ESPHOME:
        break;
    default:
        break;
    }
}
