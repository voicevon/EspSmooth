#ifndef _ROS_SERVICE_JointTrajectory_h
#define _ROS_SERVICE_JointTrajectory_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace faze4
{

static const char JOINTTRAJECTORY[] = "faze4/JointTrajectory";

  class JointTrajectoryRequest : public ros::Msg
  {
    public:
      typedef float _joint1_type;
      _joint1_type joint1;
      typedef float _joint2_type;
      _joint2_type joint2;
      typedef float _joint3_type;
      _joint3_type joint3;
      typedef float _joint4_type;
      _joint4_type joint4;
      typedef float _joint5_type;
      _joint5_type joint5;
      typedef float _joint6_type;
      _joint6_type joint6;

    JointTrajectoryRequest():
      joint1(0),
      joint2(0),
      joint3(0),
      joint4(0),
      joint5(0),
      joint6(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_joint1;
      u_joint1.real = this->joint1;
      *(outbuffer + offset + 0) = (u_joint1.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_joint1.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_joint1.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_joint1.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->joint1);
      union {
        float real;
        uint32_t base;
      } u_joint2;
      u_joint2.real = this->joint2;
      *(outbuffer + offset + 0) = (u_joint2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_joint2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_joint2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_joint2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->joint2);
      union {
        float real;
        uint32_t base;
      } u_joint3;
      u_joint3.real = this->joint3;
      *(outbuffer + offset + 0) = (u_joint3.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_joint3.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_joint3.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_joint3.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->joint3);
      union {
        float real;
        uint32_t base;
      } u_joint4;
      u_joint4.real = this->joint4;
      *(outbuffer + offset + 0) = (u_joint4.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_joint4.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_joint4.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_joint4.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->joint4);
      union {
        float real;
        uint32_t base;
      } u_joint5;
      u_joint5.real = this->joint5;
      *(outbuffer + offset + 0) = (u_joint5.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_joint5.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_joint5.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_joint5.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->joint5);
      union {
        float real;
        uint32_t base;
      } u_joint6;
      u_joint6.real = this->joint6;
      *(outbuffer + offset + 0) = (u_joint6.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_joint6.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_joint6.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_joint6.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->joint6);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_joint1;
      u_joint1.base = 0;
      u_joint1.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_joint1.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_joint1.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_joint1.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->joint1 = u_joint1.real;
      offset += sizeof(this->joint1);
      union {
        float real;
        uint32_t base;
      } u_joint2;
      u_joint2.base = 0;
      u_joint2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_joint2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_joint2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_joint2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->joint2 = u_joint2.real;
      offset += sizeof(this->joint2);
      union {
        float real;
        uint32_t base;
      } u_joint3;
      u_joint3.base = 0;
      u_joint3.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_joint3.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_joint3.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_joint3.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->joint3 = u_joint3.real;
      offset += sizeof(this->joint3);
      union {
        float real;
        uint32_t base;
      } u_joint4;
      u_joint4.base = 0;
      u_joint4.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_joint4.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_joint4.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_joint4.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->joint4 = u_joint4.real;
      offset += sizeof(this->joint4);
      union {
        float real;
        uint32_t base;
      } u_joint5;
      u_joint5.base = 0;
      u_joint5.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_joint5.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_joint5.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_joint5.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->joint5 = u_joint5.real;
      offset += sizeof(this->joint5);
      union {
        float real;
        uint32_t base;
      } u_joint6;
      u_joint6.base = 0;
      u_joint6.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_joint6.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_joint6.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_joint6.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->joint6 = u_joint6.real;
      offset += sizeof(this->joint6);
     return offset;
    }

    const char * getType(){ return JOINTTRAJECTORY; };
    const char * getMD5(){ return "b0068422b386ec8608fa05052ffe139f"; };

  };

  class JointTrajectoryResponse : public ros::Msg
  {
    public:
      typedef const char* _output_type;
      _output_type output;
      typedef int64_t _sum_type;
      _sum_type sum;

    JointTrajectoryResponse():
      output(""),
      sum(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_output = strlen(this->output);
      varToArr(outbuffer + offset, length_output);
      offset += 4;
      memcpy(outbuffer + offset, this->output, length_output);
      offset += length_output;
      union {
        int64_t real;
        uint64_t base;
      } u_sum;
      u_sum.real = this->sum;
      *(outbuffer + offset + 0) = (u_sum.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sum.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_sum.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_sum.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_sum.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_sum.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_sum.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_sum.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->sum);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_output;
      arrToVar(length_output, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_output; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_output-1]=0;
      this->output = (char *)(inbuffer + offset-1);
      offset += length_output;
      union {
        int64_t real;
        uint64_t base;
      } u_sum;
      u_sum.base = 0;
      u_sum.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sum.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_sum.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_sum.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_sum.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_sum.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_sum.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_sum.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->sum = u_sum.real;
      offset += sizeof(this->sum);
     return offset;
    }

    const char * getType(){ return JOINTTRAJECTORY; };
    const char * getMD5(){ return "6476c66121204f2ed13e150d82133200"; };

  };

  class JointTrajectory {
    public:
    typedef JointTrajectoryRequest Request;
    typedef JointTrajectoryResponse Response;
  };

}
#endif
