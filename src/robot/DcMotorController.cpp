#include "DcMotorController.h"



DcMotorController::DcMotorController(esphome::ads1115::ADS1115Sensor* ads1115_sensor, DcMotor* dc_motor){
    __ads1115_sensor = ads1115_sensor;
    __dc_motor = dc_motor;

}

void DcMotorController::loop(){
    float adc = __ads1115_sensor->sample();
    // convert voltage to angle
    float sensor_angle = adc;
    float target_angle = 1234.56;
    float output = __pid_controller->get_output_value(target_angle,sensor_angle);
    __dc_motor->goto_position(output);
}