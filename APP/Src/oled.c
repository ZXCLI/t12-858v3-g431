#include "oled.h"



// 通过寄存器操作spi
void SpiTransmit(uint8_t data)
{
	*((uint8_t *)&(SPI2->DR) + 1) = data;//只往低八位赋值，不然低八位发完还会接着发送一个0x00(高八位被发出来了)
    while (SPI2->SR & SPI_SR_BSY);
    // while ((SPI2->SR & (1 << 1)) == 0);   // 等待发送缓冲区为空
    // *((uint8_t *)&(SPI2->DR) + 1) = data; // 只往低八位赋值，不然低八位发完还会接着发送一个0x00(高八位被发出来了)，好像只是f0系列的bug
    // while ((SPI2->SR & 6272));
    // HAL_SPI_Transmit(&hspi2, &data, 1, 100);
}

/*OLED硬件复位
 */
void OLED_Reset(void)
{
	OLED_RST_CLR;
	HAL_Delay(10);
	OLED_RST_SET;
	HAL_Delay(5);
}

/*发送8位命令寄存器地址 Add
 */
void OLED_WriteAdd(uint8_t Add)
{
	OLED_DC_CLR;
	SpiTransmit(Add);
}

/*向OLED发送一个8位数据 data
 */
void OLED_WriteData(uint8_t data)
{
    OLED_DC_SET;
	SpiTransmit(data);
}

/*
 *清屏
 */
void OLED_Clear(void)
{
	uint8_t i, n;

	for (i = 0; i < 8; i++)
	{
        //sh1106
		OLED_WriteAdd(i + 0xb0); // 设置页地址（0~7）
		OLED_WriteAdd(0x01);	 // 设置显示位置—列低地址
		OLED_WriteAdd(0x10);	 // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
		{
			OLED_WriteData(0x00);
		}
	}
}

void OLED_Init(void)
{
	OLED_CS_CLR;
	OLED_Reset();
// //sh1106
// 	OLED_WriteAdd(0xAE); // set display display ON/OFF,AFH/AEH
// 	OLED_WriteAdd(0x20);
// 	OLED_WriteAdd(0x01);
// 	// set display start line:COM0
// 	OLED_WriteAdd(0x40);
// 	OLED_WriteAdd(0xB0);
// 	// 亮度设置
// 	OLED_WriteAdd(0x81);
// 	OLED_WriteAdd(0xFF); // 该数字越大，OLED越亮

// 	OLED_WriteAdd(0xA1); // entire display on: A4H:OFF/A5H:ON
// 	OLED_WriteAdd(0xC8); // 该指令控制显示方向显示方向0xc8或者0xc0
// 	// OLED_WriteAdd(0xC0);

// 	OLED_WriteAdd(0xA8); // set multiplex ratio
// 	OLED_WriteAdd(0x3F); // 1/64duty
// 	OLED_WriteAdd(0xD3); // set display offset
// 	OLED_WriteAdd(0x00); //
// 	OLED_WriteAdd(0xD5); // set display clock divide ratio/oscillator frequency
// 	OLED_WriteAdd(0x80); // 105Hz
// 	OLED_WriteAdd(0xD9); // Dis-charge /Pre-charge Period Mode Set
// 	OLED_WriteAdd(0xF1); //
// 	OLED_WriteAdd(0xDA); // Common Pads Hardware Configuration Mode Set
// 	OLED_WriteAdd(0x12); //
// 	OLED_WriteAdd(0xDB); // set vcomh deselect level
// 	OLED_WriteAdd(0x40); // VCOM = β X VREF = (0.430 + A[7:0] X 0.006415) X VREF
// 	OLED_WriteAdd(0xA4);
// 	OLED_WriteAdd(0xA6);
// 	OLED_WriteAdd(0xAF); // set display display ON/OFF,AEH/AFH

// 	OLED_WriteAdd(0xAF);
// 	OLED_WriteAdd(0xA6); // set normal/inverse display: 0xA6:正显/0xA7:反显

// 	OLED_Clear();
// 	OLED_WriteAdd(0xb0); // 设置页
// 	OLED_WriteAdd(0x10); // 设置列的显示位置的高位
// 	OLED_WriteAdd(0x01); // 低位


//ssd1306
    OLED_WriteAdd(0xAE); // 关闭显示
    OLED_WriteAdd(0xD5); // 设置时钟分频因子
    OLED_WriteAdd(80);

    OLED_WriteAdd(0xA8); // 设置驱动路数
    OLED_WriteAdd(0x3F); // 路数默认0x3F（1/64）

    OLED_WriteAdd(0xD3); // 设置显示偏移
    OLED_WriteAdd(0x00); // 偏移默认为0

    OLED_WriteAdd(0x40); // 设置显示开始行[5:0]

    OLED_WriteAdd(0x8D); // 电荷泵设置
    OLED_WriteAdd(0x14); // bit2，开启/关闭

    OLED_WriteAdd(0x20); // 设置内存地址模式
    OLED_WriteAdd(0x00); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;

    OLED_WriteAdd(0xA1); // 段重定义设置,bit0:0,0->0;1,0->127;
    OLED_WriteAdd(0xC8); // 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    //A0,C0是反转180度显示

    OLED_WriteAdd(0xDA); // 设置COM硬件引脚配置
    OLED_WriteAdd(0x12); //[5:4]配置

    OLED_WriteAdd(0x81); // 对比度设置
    OLED_WriteAdd(0xEF); // 默认0x7F（范围1~255，越大越亮）

    OLED_WriteAdd(0xD9); // 设置预充电周期
    OLED_WriteAdd(0xF1); //[3:0],PHASE 1;[7:4],PHASE 2;

    OLED_WriteAdd(0xDB); // 设置VCOMH 电压倍率
    OLED_WriteAdd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WriteAdd(0xA4); // 全局显示开启;bit0:1(A5),开启;0(A4),关闭;(白屏/黑屏)
    OLED_WriteAdd(0xA6); // 设置显示方式;bit0:1(A7),反相显示;0(A6),正常显示

    OLED_WriteAdd(0xAF); // 开启显示

	OLED_Clear();
	OLED_WriteAdd(0xb0); // 设置页
	OLED_WriteAdd(0x10); // 设置列的显示位置的高位
	OLED_WriteAdd(0x01); // 低位
}