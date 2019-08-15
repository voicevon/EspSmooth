// #include "ActuatorMovement.h"
#include "smoothie/robot/Robot.h"
#include "Actuator.h"
#include "ServoMotor.h"
#include "StepperMotor.h"
#include "DcMotor.h"
#include "XuefengMotor.h"

int16_t actuators_position[6];
float float_value;

void robot_motors_movement(TimerHandle_t xTimer){
    ServoMotor* servoMotor;
    DcMotor* dcMotor;
    XuefengMotor* xuefengMotor;
    // printf("[D][TimerTask][ControlMotors] at entrance.\n");
    Robot* robot = Robot::getInstance();
    for(int i=0; i<3;i++){
        Actuator* actuator = robot->actuators[i];
        float target_position = actuator->get_current_position();
        actuators_position[i]= target_position; //For mqtt sensor
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
}

void robot_motors_movement_setup(int interval_ms){
    int id = 1;
    TimerHandle_t tmr = xTimerCreate("robot_motors_movement", pdMS_TO_TICKS(interval_ms), pdTRUE, ( void * )id, 
                                    &robot_motors_movement);
    if( xTimerStart(tmr, 10 ) != pdPASS ) {
        printf("[E][setup] Timer for ControlMotors  start error. \n");
    }
    printf("[D][main] Create xTimerTask COntrolMotors is started.\n" );
}