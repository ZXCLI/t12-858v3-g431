#ifndef DEFAULT_H
#define DEFAULT_H

#include "loop.h"

#define IRON_MIN_PWM 2                             // 烙铁加热的最小占空比
#define IRON_MAX_PWM (4096 - 100)                         // 烙铁加热的最大占空比
#define ADC_NUM 4                                  // adc通道数,只启用前4个通道
#define MIN_CurrentPWM 400                        // 烙铁，风扇电流采样的最小占空比

#define IRON_PWM TIM1->CCR1
#define AUX_CHANNEL TIM1->CCR3
#define FAN_PWM TIM3->CCR1
#define GUN_PWM TIM2->CCR4
#define BLANK_PWM TIM1->CCR2



#define IRON_TEMP_ADCRead       LL_ADC_INJ_ReadConversionData12(ADC1,LL_ADC_INJ_RANK_1)
#define IRON_Current_ADCRead    LL_ADC_INJ_ReadConversionData12(ADC1,LL_ADC_INJ_RANK_2)
#define GUN_TEMP_ADCRead        LL_ADC_INJ_ReadConversionData12(ADC2,LL_ADC_INJ_RANK_1)
#define FAN_Current_ADCRead     LL_ADC_INJ_ReadConversionData12(ADC2,LL_ADC_INJ_RANK_2)

extern float DEF_IronKalaPar[];
extern float DEF_GunKalaPar[];
extern float DEF_IronLowPar[];
extern float DEF_GunLowPar[];

extern float DEF_IronConverCoeff[];
extern float DEF_GunConverCoeff[];
extern float DEF_IronPidPar[];
extern float DEF_GunPidPar[];

extern uint16_t DEF_IronTemp; 
extern uint16_t DEF_GunTemp;
extern uint8_t DEF_IronTip;
extern uint8_t DEF_GunFanDuty;

#endif
