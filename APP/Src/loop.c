#include "loop.h"

Devices Iron,Gun,*GLOBAL_DEV;
uint32_t PWM_Count;
int8_t samplingDead = 16;


// /*
//  *各种初始化
//  */
void MY_Init(void)
{
    //SPI2->CR1 &= ~SPI_CR1_BR;          // 设置spi分频系数为2
    __HAL_SPI_ENABLE(&hspi2);                                           	// 开启SPI

    OLED_Init();                                                        	// 初始化OLED

    EMAVG_reset(&Iron.lowpass);
    EMAVG_reset(&Iron.lowpass_temp);
    EMAVG_reset(&Iron.lowpass_current);
    EMAVG_reset(&Gun.lowpass);
    EMAVG_reset(&Gun.lowpass_temp);
    EMAVG_reset(&Gun.lowpass_current);

    DCL_resetPI(&Iron.DEvicePI);
    DCL_resetPI(&Gun.DEvicePI);

    LL_TIM_EnableCounter(TIM1);                               // 使能计数器
    LL_TIM_EnableCounter(TIM3);                               // 使能计数器
    LL_TIM_EnableAllOutputs(TIM1);
    LL_TIM_CC_EnableChannel(TIM1,LL_TIM_CHANNEL_CH1);
    LL_TIM_CC_EnableChannel(TIM1,LL_TIM_CHANNEL_CH2);
    LL_TIM_CC_EnableChannel(TIM1,LL_TIM_CHANNEL_CH3);

    LL_TIM_EnableIT_CC3(TIM1);
    LL_TIM_EnableIT_UPDATE(TIM3);

    LL_ADC_Enable(ADC1);  //使能ADC
    LL_ADC_Enable(ADC2);  //使能ADC

    LL_ADC_INJ_StartConversion(ADC1);
    LL_ADC_INJ_StartConversion(ADC2);
    LL_ADC_EnableIT_JEOS(ADC1);

    // TIM1->CCR1 = 1024;
    // TIM1->CCR2 = 1024+200;
    TIM1->CCR3 = 1024+2000;

    key_init();

    GlobalVariablesInit();          // 全局变量初始化
    route_to(&(about_page_instance.page));
}

void MY_Loop(void)
{
    current_page->update_ui();
    __IO int32_t encoder_diff = EncoderEvent[ENCODER_1].count;
    if(encoder_diff != 0){
        EncoderEvent[ENCODER_1].count = 0;
        current_page->on_encoder_changed(-encoder_diff);
    }
    dispatch_keys(current_page->key_handlers);
}


void GlobalVariablesInit(void)
{
    Iron.DEvicePI.Kp = 4.1f;
    Iron.DEvicePI.Ki = 0.001f;
    Iron.DEvicePI.Umax = 0.99f;
    Iron.DEvicePI.Umin = 0.004f;
    Iron.Temp_Ref_pu = 0.01f;
    Iron.start = 1;

    EMAVG_config(&Iron.lowpass,0.05f);
    EMAVG_config(&Iron.lowpass_temp,0.01f);
    EMAVG_config(&Iron.lowpass_current,0.01f);
}

void RunLoop(Devices *dev ,float Temp_pu,float Temp_Ref_pu)
{
    if(dev->start && dev->Connected){
        dev->PwmPulse = 4096.0f * DCL_runPI_C6(&dev->DEvicePI,Temp_Ref_pu,Temp_pu);
    }else{
        dev->PwmPulse = 0;
    }
}

inline void ADC_handle(void)
{
    if(PWM_Count == 1){
        //Iron.PwmPulse = IRON_PWM;
        IRON_PWM = MIN_CurrentPWM;
        FAN_PWM = MIN_CurrentPWM;
        AUX_CHANNEL = MIN_CurrentPWM>>1;             // 下个周期测电流
    }else if(PWM_Count == 2){
        Iron.Current = IRON_Current_ADCRead;
        Gun.Current = FAN_Current_ADCRead;

        Iron.Connected = Iron.Current > 200 ? 1 : 0;
        Gun.Connected = Gun.Current > 200 ? 1 : 0;

        IRON_PWM = 0;
        AUX_CHANNEL = 2000;          // 停几个周期测热电偶电压
    }else if(PWM_Count == 9 + samplingDead){
        Iron.Temp_Original = IRON_TEMP_ADCRead;
        Gun.Temp_Original = GUN_TEMP_ADCRead;

        Iron.Temp_pu = Iron.Temp_Original / 4096.0f;
        Gun.Temp_pu = Gun.Temp_Original / 4096.0f;

        EMAVG_run(&Iron.lowpass,Iron.Temp_pu);
        EMAVG_run(&Gun.lowpass,Gun.Temp_pu);

        Iron.Temp_pu = Iron.lowpass.out;
        Gun.Temp_pu = Gun.lowpass.out;
        // 进行环路计算
        RunLoop(&Iron,Iron.Temp_pu,Iron.Temp_Ref_pu);
        RunLoop(&Gun,Gun.Temp_pu,Gun.Temp_Ref_pu);
    
        IRON_PWM = Iron.PwmPulse;
        FAN_PWM = Gun.PwmPulse;
        AUX_CHANNEL = (Iron.PwmPulse + 4) >> 1;            // 下个周期运行DEBUG程序
        Iron.AdcReady = true;
        Gun.AdcReady = true;
    }else if(PWM_Count == 10 + samplingDead){
        SEGGER_RTT_printf(0,"%d,%d,%d\n",Iron.Temp_Original,Iron.PwmPulse,Iron.Connected*1000);
    }else if(PWM_Count == 400){
        PWM_Count -= 400;
    }

    PWM_Count++;
}
