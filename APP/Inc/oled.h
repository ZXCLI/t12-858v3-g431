#include "main.h"
#include "spi.h"

#define OLED_CS_CLR HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, RESET)
#define OLED_CS_SET  HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, SET)

#define OLED_DC_CLR  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, RESET) //低为命令
#define OLED_DC_SET  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin ,SET)   //高为数据

#define OLED_RST_CLR  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, RESET)
#define OLED_RST_SET  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, SET)


void OLED_Reset(void);
void OLED_Init(void);
void OLED_Clear(void);

void OLED_WriteAdd(uint8_t);
void OLED_WriteData(uint8_t);

#define DEBUG1_IN HAL_GPIO_WritePin(TEST1_GPIO_Port, TEST1_Pin, GPIO_PIN_SET);
#define DEBUG1_OUT HAL_GPIO_WritePin(TEST1_GPIO_Port, TEST1_Pin, GPIO_PIN_RESET);

#define DEBUG2_IN HAL_GPIO_WritePin(TEST2_GPIO_Port, TEST2_Pin, GPIO_PIN_SET);
#define DEBUG2_OUT HAL_GPIO_WritePin(TEST2_GPIO_Port, TEST2_Pin, GPIO_PIN_RESET);
/*
// void OLED_Show_Number_16(uint8_t x,uint8_t  y,uint8_t  number );
// void OLED_Show_Zimu_16(uint8_t x,uint8_t  y,uint8_t  number );
// void OLED_Show_Number_40x32(uint8_t x,uint8_t  y,uint8_t  number );
*/
