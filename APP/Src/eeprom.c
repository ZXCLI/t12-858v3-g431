/* 2023-01-30	by zxcli
 * Code version: 1.0
 * 24cxx文件管理
 * 24c08地址格式：
 * 4个块(BlockAddr:0-3)
 *    |___16页(PageAddr:0-15)
 *          |___16字节(BitAddr:0-15)
 * 设备地址
 * +-----+---+---+---+---+---+------------+------------+-----+
 * | bit | 0 | 1 | 2 | 3 | 4 | 5          | 6          | 7   |
 * +-----+---+---+---+---+---+------------+------------+-----+
 * |     | 1 | 0 | 1 | 0 | 0 | blockAddr1 | blockAddr2 | R/W |
 * +-----+---+---+---+---+---+------------+------------+-----+
 * 10100000--->0xA0
 * 
 * 数据地址
 * +-----+-------+-------+-------+-------+----------+----------+----------+----------+
 * | bit | 0     | 1     | 2     | 3     | 4        | 5        | 6        | 7        |
 * +-----+-------+-------+-------+-------+----------+----------+----------+----------+
 * |     | Page1 | Page2 | Page3 | Page4 | BitAddr1 | BitAddr2 | BitAddr3 | BitAddr4 |
 * +-----+-------+-------+-------+-------+----------+----------+----------+----------+
 * 
 * 数据帧格式
 * 数据块帧头----数据长度-----数据------数据块帧尾
 * +--------+-------------+--------+--------------+
 * | Header | Data Length |  Data  | End of Frame |
 * +--------+-------------+--------+--------------+
 * | 8bit   | 8bit        | n*8bit | 8bit         |
 * +--------+-------------+--------+--------------+
 * 数据块帧头：8位
 * 数据长度：8位，以字节为单位，大小3+n
 * 数据块帧尾：8位，为(数据块帧头|数据长度)
 * 数据头定义：
 * +-----+---+---+---+---+---+---+---+---+
 * | bit | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
 * +-----+---+---+---+---+---+---+---+---+
 * |     |   devices addr    | data addr |
 * +-----+-------------------+-----------+
 * 
 */

#include "eeprom.h"

#define AT24BaseAddr 0xA0
enum Devoces{Iron_single = 1,Gun_single,Iron_multi,Gun_multi};
enum Data_single{temper = 1,tipORfanspeed,kalma};
enum Data_multi{PID = 1,ConverCoeff};

void at24_EraseMemFull()
{
    uint8_t temp[16] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    for(uint8_t i = 0;i < 4;i++){
        for(uint8_t j = 0;j < 16;j++){
            HAL_I2C_Mem_Write(&hi2c1,AT24BaseAddr|(i<<1),j<<4,1,temp,16,0xFF);
            HAL_Delay(2);
        }
    }
}

//字节写
void at24_BitWrite(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,
                   uint8_t PageAddr,uint8_t BitAddr,uint8_t* RxData)
{
    HAL_I2C_Mem_Write(I2cHandle,AT24BaseAddr|(BlockAddr<<1),
                      (PageAddr<<4)|BitAddr,1,RxData,1,0xFF);
}

//按页写
void at24_PageWrite(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,
                    uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,
                    uint8_t* RxData)
{
    if((BitAddr+DataLength) > 16)   {return;}
    HAL_I2C_Mem_Write(I2cHandle,AT24BaseAddr|(BlockAddr<<1),
                      (PageAddr<<4)|BitAddr,1,RxData,DataLength,0xFF);
}

//任意地址，任意长度按页写
void at24_RandomWrite(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,
                      uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,
                      uint8_t* RxData)
{
    if((BitAddr+DataLength) <= 16){//数据未超过一页
        at24_PageWrite(I2cHandle,BlockAddr,PageAddr,
                       BitAddr,DataLength,RxData);
    }else{//数据超过一页
        uint8_t BitAddrOffset = 16 - BitAddr;
        uint8_t PageNum = (DataLength + BitAddr)>>4;
        at24_PageWrite(I2cHandle,BlockAddr,PageAddr,
                       BitAddr,BitAddrOffset,RxData);//第一页
        HAL_Delay(2);//给上面的操作延时，让EEPROM写入完成
        for(uint8_t i = 0;i < PageNum;i++){
            if(i == (PageNum-1)){//最后一页
                at24_PageWrite(I2cHandle,BlockAddr+((i+PageAddr+1)>>4),PageAddr+1+(i%16),
                               0x00,((BitAddr+DataLength)%16),RxData+BitAddrOffset+i*16);//最后一页
                HAL_Delay(2);
            }else{//中间的页
                at24_PageWrite(I2cHandle,BlockAddr+((i+PageAddr+1)>>4),PageAddr+1+(i%16),
                               0x00,16,RxData+BitAddrOffset+i*16);
                HAL_Delay(2);
            }
        }
    }
}
//任意地址，任意长度读
void at24_RandomRead(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,
                     uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,uint8_t* RxData)
{
    HAL_I2C_Mem_Read(I2cHandle,(AT24BaseAddr|(BlockAddr<<1))+1,(PageAddr<<4)|BitAddr,
                     1,RxData,DataLength,0xFF);
}

//floa转4个char类型数据
void floatTO4char(float data,uint8_t* buf)
{
    for(uint8_t i = 0;i<4;i++){
       buf[i]=(*((uint8_t*)(&data)+i));
    }
}
//转回来
void charTOfloat(uint8_t* buf,float* data)
{
    uint32_t temp=buf[3];
	for(int8_t i=2;i>=0;i--){
		temp=(temp<<8)|buf[i];
	}
	float *p=(float*)(&temp);//暂存
	*data=*p;
}
//uint16_t转2个char类型数据
void uint16TO2char(uint16_t data,uint8_t* buf)
{
    buf[0]=(data)&0xFF;
    buf[1]=data>>8;
}
//转回来
void charTOuint16(uint8_t* buf,uint16_t* data)
{
    *data = (uint16_t)(buf[0]|(buf[1]<<8));
}



void at24_HAL_write(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,
                    uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,uint8_t* RxData)
{
    if(DataLength == 1){
        at24_BitWrite(I2cHandle,BlockAddr,PageAddr,BitAddr,RxData);
        HAL_Delay(2);
    }else{
        at24_RandomWrite(I2cHandle,BlockAddr,PageAddr,BitAddr,DataLength,RxData);
    }
}

void at24_HAL_read(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,
                   uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,uint8_t* RxData)
{
    at24_RandomRead(I2cHandle,BlockAddr,PageAddr,BitAddr,DataLength,RxData);
}

//更新温度
void updateTemper(uint16_t NewTemper)
{
    uint8_t TxBuf[5];
    TxBuf[0] = Iron_single;
    TxBuf[1] = sizeof(uint16_t)+3;
    uint16TO2char(NewTemper,&TxBuf[2]);
    TxBuf[4] = TxBuf[0]|TxBuf[1];
    at24_HAL_write(&hi2c1,0x00,0x00,0x00,5,TxBuf);
}

uint16_t readTemper()
{
    uint8_t RxBuf[5];
    at24_HAL_read(&hi2c1,0x00,0x00,0x00,5,RxBuf);
    uint16_t temp;
    charTOuint16(&RxBuf[2],&temp);
    return temp;
}
