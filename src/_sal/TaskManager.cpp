#include "TaskManager.h"
#include "_SAL/FtpServer/ESP32FtpServer.h"
#include "esphome/core/application.h"

//===============================================================
FtpServer* __fpt_server;
extern FtpServer* ftpserver_setup();
void ftp_loop(void*){
    while(true){
        __fpt_server->handleFTP();
    }
}

//=============================================================
esphome::Application * __esphome;
extern esphome::Application* esphome_setup();
void task_esphome_loop(void*){
  while(true){
    __esphome->loop();
  }
}


//==============================================================
void Start_Task(TASK_ITEMS_T target_task){
    switch (target_task)
    {
    case FTP_SERVER:
        __fpt_server = ftpserver_setup();
        xTaskCreate(ftp_loop, "ftp_loop", 30000, NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);
        break;
    case SERIAL_COMM:

        break;
    case TCP_COMM:
        break;
    case ESPHOME:
        __esphome = esphome_setup();
        // delay(5000);   //Wait wifi connecting.  TODO:// set as a condition.
        xTaskCreate(task_esphome_loop, "esphome_loop", 30000, NULL, (tskIDLE_PRIORITY + 1UL), (TaskHandle_t *) NULL);
        break;
    default:
        break;
    }
}
