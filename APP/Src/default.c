#include "default.h"

float DEF_IronKalaPar[] =
{   0,              //P1
    0.001f,         //Q
    1.0f,           //R
    0,              //Kg
    10.0f,          //P 
    // 0.1036792f,     //k
    // 45.00654f       //b
};   
      
float DEF_GunKalaPar[] =
{   0,
    0.001f,
    1.0f,
    0,
    10.0f,
    // 0.1036792f,     //k
    // 45.00654f       //b
};         //风枪のP1,Q,R,Kg,P


float DEF_IronConverCoeff[]=
{
    45.00654,
    0.1036792
};

float DEF_GunConverCoeff[]=
{
    45.00654,
    0.1036792
};

float DEF_IronPidPar[] = 
{   24,            //kp
    0.002f,          //ki
    0.000f,              //kd
    1.0,            //kr
    1.0,            //lm
    IRON_MAX_PWM,   //Umax
    IRON_MIN_PWM    //Umin
};
float DEF_GunPidPar[] =
{   3,            //kp
    0.005,          //ki
    0,              //kd
    1.0,            //kr
    1.0,            //lm
    IRON_MAX_PWM,   //Umax
    IRON_MIN_PWM
};  //Umin

uint16_t DEF_IronTemp = 200;
uint16_t DEF_GunTemp = 240;
uint8_t DEF_IronTip = 1;
uint8_t DEF_GunFanDuty = 4;