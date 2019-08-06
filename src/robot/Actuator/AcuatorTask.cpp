#include "robot/Robot.h"
#include "robot/Actuator/Actuator.h"
#include "robot/Actuator/ServoMotor.h"
#include "robot/Actuator/StepperMotor.h"
#include "robot/Actuator/DcMotor.h"
#include "robot/Actuator/XuefengMotor.h"
// uint8_t xxx=0;
// float p[3];
// Actuator* aa;
// float fff[3];
float float_value;
void ControlMotors(TimerHandle_t xTimer){
    ServoMotor* servoMotor;
    DcMotor* dcMotor;
    XuefengMotor* xuefengMotor;
    // while(true){
        // printf("[D][TimerTask][ControlMotors] at entrance.\n");
        Robot* robot = Robot::getInstance();
        for(int i=0; i<3;i++){
            Actuator* actuator = robot->actuators[i];
            float target_position = actuator->get_current_position();
            switch (actuator->get_motor_type())
            {
            case Actuator::SERVO_MOTOR:
                servoMotor = (ServoMotor*) actuator; 
                servoMotor->goto_position(target_position);
                float_value = target_position;
                break;
            case Actuator::DC_MOTOR:
                dcMotor =(DcMotor*) actuator;
                dcMotor->pid_loop(target_position);
                break;
            case Actuator::XUEFENG_MOTOR:
                xuefengMotor = (XuefengMotor*) actuator;
                xuefengMotor->goto_position(target_position);
                break;          
            case Actuator::STEPPER_MOTOR:
                //Do nothing.

                break;
            default:
                break;
            }
        }   
        // printf(">>>>>>>>>>>>>>>>>>>>>>>>>> exiting task>>>>>>>>>>>>\n");
        // without this, will  reboot. Why? Aug 2019 Xuming 
        // possible reason1: previous PWM writing is not finished. 
        // delay(100);  //Can be shorter? faster? smoothier?
    // }
}