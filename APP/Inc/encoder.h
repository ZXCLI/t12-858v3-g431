#ifndef _ENCODER_H
#define _ENCODER_H

#include "main.h"
#include "stdbool.h"

/*--------------------------------用户需要更改的地方-----------------------*/

typedef enum
{
  	ENTER = 0,
	LEFT,
  	RIGHT,
  	KEY_COUNT,
}KEY_NAME;

typedef enum
{
  	ENCODER_1 = 0,
  	ENCODER_COUNT,
}ENCODER_NAME;


/*-------------------------------编码器--------------------------------------*/

typedef	struct
{
	__IO int32_t count;

    __IO bool left;  //左旋标志位
  	__IO bool right; //右旋标志位

  	//负责计数的变量
  	uint32_t left_timeout;  //左旋超时计数
  	uint32_t right_timeout; //右旋超时计数

  	//负责记录状态的变量
  	uint8_t state;       //当前状态
  	uint8_t last_state;  //上一次状态

}ENCODER;

typedef struct
{
    GPIO_TypeDef* GPIO_left_part;
    uint16_t GPIO_left_pin;
    GPIO_TypeDef* GPIO_right_part;
    uint16_t GPIO_right_pin;

}ENCODER_GPIO;
/*-------------------------------按键---------------------------------------*/
typedef struct
{
    __IO bool KeyPressed;    //按键按下标志位
    __IO bool shortPress;    //短按标志位
    __IO bool longPress;     //长按标志位

    //负责计数的变量
    uint32_t press_count;   //按下时间计数

    //及时清除未被读取的标志位的计数变量：
    uint32_t short_timeout; //短按超时计数                   
    uint32_t long_timeout;  //长按超时计数

    //负责记录状态的变量
    uint8_t state;       //当前状态
    uint8_t last_state;  //上一次状态

}KEY;

typedef struct
{
    GPIO_TypeDef* GPIO_part;
    uint16_t GPIO_pin;

}KEY_GPIO;


extern KEY KeyEvent[KEY_COUNT];
extern ENCODER EncoderEvent[ENCODER_COUNT];


uint8_t encoder_read(uint8_t snack);
void encoder_poll(void);

void key_init();
uint8_t key_read(uint8_t snack);
void key_poll();

void timeout(uint32_t* TimeCount,bool* State);

#endif
