
// #include "ROS/faze4/RobotFaze4_ros.h"

// #include "ros.h"
// #include "robot.h"
// ros::NodeHandle  nh;

// Robot::STATE current_state;
// // Publisher
// // std_msgs::String str_msg;
// PACKAGE_NAME::RobotState ros_msg;
// ros::Publisher publisher_agent(TOPIC_STRING , &ros_msg);

// void follow_joint_trajectory(const PACKAGE_NAME::JointTrajectory::Request & req, PACKAGE_NAME::JointTrajectory::Response & res){
//     long abs_steps[6];  // Array of desired stepper positions must be long
//     abs_steps[1-1] = req.joint1;
//     abs_steps[2-1] = req.joint2;
//     abs_steps[3-1] = req.joint3; 
//     abs_steps[4-1] = req.joint4; 
//     abs_steps[5-1] = req.joint5;
//     abs_steps[6-1] = req.joint6;

//     ROBOT_CLASS_NAME::get_instance().move_to(abs_steps);
//     res.output = "OK";
    
//     // Change state and publish a message
//     current_state =  Robot::STATE::MOVING;
//     ros_msg.state = "MOVING";
//     publisher_agent.publish( &ros_msg);
// }

// void gripper_command(const PACKAGE_NAME::GripperCommand::Request & req, PACKAGE_NAME::GripperCommand::Response & res){
//     ROBOT_CLASS_NAME::get_instance().hand_do(req.action);

// }
// // Service
// ros::ServiceServer<PACKAGE_NAME::JointTrajectory::Request, PACKAGE_NAME::JointTrajectory::Response> arm_server("FollowJointTrajectory", &follow_joint_trajectory);
// ros::ServiceServer<PACKAGE_NAME::GripperCommand::Request, PACKAGE_NAME::GripperCommand::Response> hand_server("GripperCommand_service", &gripper_command);

// void setup_ros(){
//     ROBOT_CLASS_NAME::get_instance().init();
//     nh.initNode();
//     nh.getHardware()->setBaud(57600);
//     // ROBOT_CLASS_NAME::get_instance().process_homing_one_by_one();

//     Serial.println("================ ROBOT Ardunino ROS Node =================");
//     Serial.println("Starting publisher");
//     nh.advertise(publisher_agent);
//     Serial.println("Starting services");
//     nh.advertiseService(arm_server);   //Should send something to serial port, right? NO!
//     nh.advertiseService(hand_server);  //Should send something to serial port, right? NO!
//     Serial.println("publisher and Services is started.");
    
//     current_state = Robot::STATE::HOMING;
// }

// void loop_ros()
// {
//     switch (current_state){
//         case Robot::STATE::HOMING:
//             ROBOT_CLASS_NAME::get_instance().process_homing_one_by_one(); 
//             ros_msg.state = "Idle";
//             publisher_agent.publish(&ros_msg);
//             break;
//         case Robot::STATE::IDLE:
//             // might be updated to MOVING in follow_joint_trajectory() service.
//             break;
//         case Robot::STATE::MOVING:
//             // check all motors, if all are stopped, change state to idle
//             if( !ROBOT_CLASS_NAME::get_instance().is_moving() ){
//                 current_state = Robot::STATE::IDLE;
//                 ros_msg.state = "IDLE";
//                 publisher_agent.publish( &ros_msg );
//             }
//             break;        
//         default:
//             break;
//     }
//     nh.spinOnce();
//     delay(100);
// }
