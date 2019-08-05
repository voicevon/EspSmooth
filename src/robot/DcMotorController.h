#pragma once
#include "component/ads1115.h"
#include "robot/Actuator/DcMotor.h"
#include "libs/MotorPidController.h"


class DcMotorController{

    public:
        DcMotorController(esphome::ads1115::ADS1115Sensor* ads1115_sensor, DcMotor* dc_motor);
        void loop();
        

    private:
        esphome::ads1115::ADS1115Sensor* __ads1115_sensor;
        DcMotor* __dc_motor;
        MotorPidController* __pid_controller;
};