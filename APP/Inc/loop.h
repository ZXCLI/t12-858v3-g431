#ifndef _LOOP_H
#define _LOOP_H

#include "OLEDUI.h"
#include "IQmathLib.h"
#include "stdbool.h"
#include "string.h"
#include "SEGGER_RTT.h"
// #include "menu.h"
#include "encoder.h"
#include "eeprom.h"
#include "default.h"
#include "DCL.h"
#include "emavg.h"
#include "menu_page.h"
#include "setting_page.h"

//#include "dma.h"
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
// #include "can.h"

typedef struct 
{
    float Temp_pu;                                  //温度，归一化
    float Temp_Ref_pu;                              //溫度參考值，归一化
    uint16_t Temp_Original;                         //ADC直读，未经换算的温度数值，用于校准
    uint16_t Current;                               //电流(烙铁&&风扇)，用于判断设备是否连接
    uint16_t PwmPulse;                              //占空比(烙铁&&风枪加热丝)
    uint16_t FAN_PwmPulse;                          //占空比(风扇)
    bool AdcReady;                                  //温度和电流采样完成准备计算PID
    bool Connected;                                 //设备已经正确连接
    bool start;                                     //开始运行加热
    bool enter_menu;                                //是否进入设置菜单
    EMAVG lowpass;                                  //滤波数据结构体
    EMAVG lowpass_current;                          //电流滤波数据结构体
    EMAVG lowpass_temp;                             //温度滤波数据结构体
    // DataConversion DataConverCoeff;                 //数据换算系数
    DCL_PI DEvicePI;                                //PID数据结构体
    uint8_t tips_index;                             //当前选择的烙铁头类型
}Devices;

extern Devices Iron,Gun;

void MY_Loop(void);
void MY_Init(void);
void ADC_handle(void);
void IRON_UI();
void GUN_UI();

void GlobalVariablesInit(void);
//算环路
void RunLoop(Devices *dev ,float Temp_pu,float Temp_Ref_pu);

#endif