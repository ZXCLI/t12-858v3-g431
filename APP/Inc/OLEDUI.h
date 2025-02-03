#include "main.h"
#include "oled.h"
#include "stdbool.h"
#include "string.h"

typedef struct COORDINATE 
{
	int x;
	int y;
}Type;

struct Cn16CharTypeDef                	// 汉字字模数据结构 
{
	unsigned char  Index[2];            // 汉字内码索引,一个汉字占两个字节	
	unsigned char  Msk[32];             // 点阵码数据(16*16有32个数据) 
};

void OLED_CleanBuffer(void);
void flash(void);

unsigned char pgm_read_byte(const unsigned char *addr);
unsigned int oled_pow(unsigned char m,unsigned char n);
void SetPointBuffer(uint8_t , uint8_t ,uint8_t);

void FillRect(int x,int y,int width,int height);

void DrawMYfillRect(int x, int y, uint8_t w, uint8_t h);
void DrawMYfillRoundRect(int x, int y, uint8_t w, uint8_t h);

void DrawPiel(uint8_t , uint8_t ,uint8_t);
void DrawLine(uint8_t , uint8_t , uint8_t , uint8_t );
void DrawFastHLine(uint8_t , uint8_t , uint8_t );
void DrawFastVLine(uint8_t , uint8_t , uint8_t );
void DrawRect1(int left,int top,int right,int bottom);
void DrawCircleHelper(uint8_t,uint8_t, unsigned char , unsigned char);
void DrawRoundRect(uint8_t , uint8_t , unsigned char w, unsigned char h, unsigned char r);
void DrawfillRoundRect(uint8_t x, uint8_t y, unsigned char w, unsigned char h, unsigned char r);
void DrawChar(uint8_t , uint8_t , unsigned char c,uint8_t size,uint8_t bg);
void DrawString(uint8_t x, uint8_t y, char *str,uint8_t size);
void DrawNum(uint8_t x, uint8_t y, uint16_t num,uint8_t len,uint8_t size,uint8_t bg);
void DrawHalfRoundRect(uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint8_t direction);
void DrawMYflaotRoundRect(int x, int y, uint8_t w, uint8_t h);
void MY_FillRect(int x,int y,int width,int height);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t *cn);
