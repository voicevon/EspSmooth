#pragma once

enum TASK_ITEMS_T { 
      ESPHOME
    , FTP_SERVER
    , TCP_SERVER
    , SERIAL_COMM  // This is not the debug serial port, just comm for end users.
    , TCP_COMM
    , ROBOT
    
};

enum TIMER_TASK_ITEMS_T{
    CONTROL_ROBOT_MOTORS
    , WEEDER

};
extern void Start_TimerTask(TIMER_TASK_ITEMS_T);
extern void Start_Task(TASK_ITEMS_T targetTask);