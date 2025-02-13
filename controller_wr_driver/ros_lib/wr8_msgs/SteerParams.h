#ifndef _ROS_SERVICE_SteerParams_h
#define _ROS_SERVICE_SteerParams_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace wr8_msgs
{

static const char STEERPARAMS[] = "wr8_msgs/SteerParams";

  class SteerParamsRequest : public ros::Msg
  {
    public:
      typedef float _left_k_type;
      _left_k_type left_k;
      typedef float _right_k_type;
      _right_k_type right_k;

    SteerParamsRequest():
      left_k(0),
      right_k(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_left_k;
      u_left_k.real = this->left_k;
      *(outbuffer + offset + 0) = (u_left_k.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_left_k.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_left_k.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_left_k.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->left_k);
      union {
        float real;
        uint32_t base;
      } u_right_k;
      u_right_k.real = this->right_k;
      *(outbuffer + offset + 0) = (u_right_k.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_right_k.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_right_k.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_right_k.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->right_k);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_left_k;
      u_left_k.base = 0;
      u_left_k.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_left_k.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_left_k.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_left_k.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->left_k = u_left_k.real;
      offset += sizeof(this->left_k);
      union {
        float real;
        uint32_t base;
      } u_right_k;
      u_right_k.base = 0;
      u_right_k.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_right_k.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_right_k.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_right_k.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->right_k = u_right_k.real;
      offset += sizeof(this->right_k);
     return offset;
    }

    const char * getType(){ return STEERPARAMS; };
    const char * getMD5(){ return "df62dc88325eb30b6890e3d42813c81e"; };

  };

  class SteerParamsResponse : public ros::Msg
  {
    public:
      typedef int8_t _dummy_type;
      _dummy_type dummy;

    SteerParamsResponse():
      dummy(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_dummy;
      u_dummy.real = this->dummy;
      *(outbuffer + offset + 0) = (u_dummy.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->dummy);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int8_t real;
        uint8_t base;
      } u_dummy;
      u_dummy.base = 0;
      u_dummy.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->dummy = u_dummy.real;
      offset += sizeof(this->dummy);
     return offset;
    }

    const char * getType(){ return STEERPARAMS; };
    const char * getMD5(){ return "f19c17840e71ac5736972b2b4d4c6188"; };

  };

  class SteerParams {
    public:
    typedef SteerParamsRequest Request;
    typedef SteerParamsResponse Response;
  };

}
#endif
