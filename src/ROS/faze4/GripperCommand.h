// #ifndef _ROS_SERVICE_GripperCommand_h
// #define _ROS_SERVICE_GripperCommand_h
// #include <stdint.h>
// #include <string.h>
// #include <stdlib.h>
// #include "ros/msg.h"

// namespace faze4
// {

// static const char GRIPPERCOMMAND[] = "faze4/GripperCommand";

//   class GripperCommandRequest : public ros::Msg
//   {
//     public:
//       typedef uint8_t _action_type;
//       _action_type action;

//     GripperCommandRequest():
//       action(0)
//     {
//     }

//     virtual int serialize(unsigned char *outbuffer) const
//     {
//       int offset = 0;
//       *(outbuffer + offset + 0) = (this->action >> (8 * 0)) & 0xFF;
//       offset += sizeof(this->action);
//       return offset;
//     }

//     virtual int deserialize(unsigned char *inbuffer)
//     {
//       int offset = 0;
//       this->action =  ((uint8_t) (*(inbuffer + offset)));
//       offset += sizeof(this->action);
//      return offset;
//     }

//     const char * getType(){ return GRIPPERCOMMAND; };
//     const char * getMD5(){ return "f1d6170759a11ac69d62121271b22bc9"; };

//   };

//   class GripperCommandResponse : public ros::Msg
//   {
//     public:
//       typedef int64_t _sum_type;
//       _sum_type sum;

//     GripperCommandResponse():
//       sum(0)
//     {
//     }

//     virtual int serialize(unsigned char *outbuffer) const
//     {
//       int offset = 0;
//       union {
//         int64_t real;
//         uint64_t base;
//       } u_sum;
//       u_sum.real = this->sum;
//       *(outbuffer + offset + 0) = (u_sum.base >> (8 * 0)) & 0xFF;
//       *(outbuffer + offset + 1) = (u_sum.base >> (8 * 1)) & 0xFF;
//       *(outbuffer + offset + 2) = (u_sum.base >> (8 * 2)) & 0xFF;
//       *(outbuffer + offset + 3) = (u_sum.base >> (8 * 3)) & 0xFF;
//       *(outbuffer + offset + 4) = (u_sum.base >> (8 * 4)) & 0xFF;
//       *(outbuffer + offset + 5) = (u_sum.base >> (8 * 5)) & 0xFF;
//       *(outbuffer + offset + 6) = (u_sum.base >> (8 * 6)) & 0xFF;
//       *(outbuffer + offset + 7) = (u_sum.base >> (8 * 7)) & 0xFF;
//       offset += sizeof(this->sum);
//       return offset;
//     }

//     virtual int deserialize(unsigned char *inbuffer)
//     {
//       int offset = 0;
//       union {
//         int64_t real;
//         uint64_t base;
//       } u_sum;
//       u_sum.base = 0;
//       u_sum.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
//       u_sum.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
//       u_sum.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
//       u_sum.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
//       u_sum.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
//       u_sum.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
//       u_sum.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
//       u_sum.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
//       this->sum = u_sum.real;
//       offset += sizeof(this->sum);
//      return offset;
//     }

//     const char * getType(){ return GRIPPERCOMMAND; };
//     const char * getMD5(){ return "b88405221c77b1878a3cbbfff53428d7"; };

//   };

//   class GripperCommand {
//     public:
//     typedef GripperCommandRequest Request;
//     typedef GripperCommandResponse Response;
//   };

// }
// #endif
