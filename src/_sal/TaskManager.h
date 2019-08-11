#pragma once

enum TASK_ITEMS_T 
{ FTP_SERVER
, SERIAL_COMM
, TCP_COMM
, ROBOT
, ESPHOME
};


extern void Start_Task(TASK_ITEMS_T targetTask);