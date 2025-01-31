#include "encoder.h"

uint32_t longpress_count,shortpress_count;

KEY KeyEvent[KEY_COUNT];
ENCODER EncoderEvent[ENCODER_COUNT];

/*-----------------------------------用户需要更改的地方--------------------------*/

KEY_GPIO KeyEvent_GPIO[KEY_COUNT] = {
    {ENCODE_S_GPIO_Port,ENCODE_S_Pin},//ENTER
    {KEY1_GPIO_Port,KEY1_Pin},//LEFT
    {KEY2_GPIO_Port,KEY2_Pin},//RIGHT
};

ENCODER_GPIO EncoderEvent_GPIO[ENCODER_COUNT] = {
    {
        ENCODE_L_GPIO_Port,ENCODE_L_Pin,
        ENCODE_R_GPIO_Port,ENCODE_R_Pin
    }
};
/*------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  编码器相关
------------------------------------------------------------------------------*/
inline uint8_t encoder_read(uint8_t snack)
{
    return (((HAL_GPIO_ReadPin(EncoderEvent_GPIO[snack].GPIO_left_part,
                               EncoderEvent_GPIO[snack].GPIO_left_pin))<<1)
            |(HAL_GPIO_ReadPin(EncoderEvent_GPIO[snack].GPIO_right_part,
                               EncoderEvent_GPIO[snack].GPIO_right_pin)))&0x3;
}

void encoder_poll()
{
    for(uint8_t snack = 0;snack < ENCODER_COUNT;snack++){

        EncoderEvent[snack].state = ((EncoderEvent[snack].last_state << 2) 
                                     | encoder_read(snack))&0xf;

        if(EncoderEvent[snack].state == 0x08){
            //正转
            EncoderEvent[snack].left = true;
            EncoderEvent[snack].count++;
        }else if(EncoderEvent[snack].state == 0x04){
            //反转
            EncoderEvent[snack].right = true;
            EncoderEvent[snack].count--;
        }

        EncoderEvent[snack].last_state = EncoderEvent[snack].state;

        timeout(&EncoderEvent[snack].left_timeout, &EncoderEvent[snack].left);
        timeout(&EncoderEvent[snack].right_timeout, &EncoderEvent[snack].right);
    }
}

/*------------------------------------------------------------------------------
  按键相关
------------------------------------------------------------------------------*/
void key_init()
{
    shortpress_count = 200;
    longpress_count = 12000; 
}

inline uint8_t key_read(uint8_t snack)
{
    return HAL_GPIO_ReadPin(KeyEvent_GPIO[snack].GPIO_part,
                            KeyEvent_GPIO[snack].GPIO_pin);
}

void key_poll(void)
{
    for(uint8_t snack = 0;snack < KEY_COUNT;snack++){
        KeyEvent[snack].state = ((KeyEvent[snack].last_state << 1) 
                                 | key_read(snack))&0x03;
        switch(KeyEvent[snack].state) {
            case 0x02://按下的一瞬间
                KeyEvent[snack].KeyPressed = false;
                break;
            case 0x01://抬起的一瞬间
                if(KeyEvent[snack].press_count >= longpress_count){
                    KeyEvent[snack].KeyPressed = false;
                    KeyEvent[snack].press_count = 0;
                    break;
                }else if((KeyEvent[snack].press_count >= shortpress_count)){
                    KeyEvent[snack].shortPress = true;
                    KeyEvent[snack].KeyPressed = false;
                    KeyEvent[snack].press_count = 0;
                    break;
                }
                break;
            case 0x00://处于按下状态
                KeyEvent[snack].press_count++;
                if(KeyEvent[snack].press_count >= longpress_count){     
                    //直接处理长按事件，如果在抬起时才处理会感觉卡顿
                    KeyEvent[snack].longPress = true;
                    break;
                }
                KeyEvent[snack].KeyPressed = true;
                break;
            case 0x03://处于松开状态，进行超时处理
                KeyEvent[snack].KeyPressed = false;
                timeout(&KeyEvent[snack].short_timeout,&KeyEvent[snack].shortPress);
                timeout(&KeyEvent[snack].long_timeout,&KeyEvent[snack].longPress);
                break;
        }
        KeyEvent[snack].last_state = KeyEvent[snack].state;
    }
}

//未处理的按键和编码器标志位超时清零
inline void timeout(uint32_t* TimeCount,bool* State)
{
    if(((*TimeCount) >= 400)&&(*State)){       
        (*State) = false;
        (*TimeCount) = 0;
    }
    if(*State){(*TimeCount)++;}
    
    if(*TimeCount > 4000){      //防止出现BUG
        (*TimeCount) = 0;
        (*State) = false;
    }
}
