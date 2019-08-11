#pragma once

enum TASK_ITEMS_T { 
    FTP_SERVER
    , SERIAL_COMM  // This is not the debug serial port, just comm for end users.
    , TCP_COMM
    , ROBOT
    , ESPHOME
};

enum TIMER_TASK_ITEMS_T{
    CONTROL_ROBOT_MOTORS

};
extern void Start_TimerTask(TIMER_TASK_ITEMS_T);
extern void Start_Task(TASK_ITEMS_T targetTask);