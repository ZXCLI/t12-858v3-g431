#ifndef _EEPROM_H
#define _EEPROM_H

#include "main.h"
#include "i2c.h"

void at24_EraseMemFull(void);
void at24_RandomRead(I2C_HandleTypeDef* handle,uint8_t BlockAddr,uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,uint8_t* RxData);
void at24_BitWrite(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,uint8_t PageAddr,uint8_t BitAddr,uint8_t* RxData);
void at24_PageWrite(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,uint8_t* RxData);

void floatTO4char(float data,uint8_t* out);
void charTOfloat(uint8_t* buf,float* data);
void uint16TO2char(uint16_t data,uint8_t* buf);
void charTOuint16(uint8_t* buf,uint16_t* data);

//给外界的接口
void at24_HAL_write(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,uint8_t* RxData);
void at24_HAL_read(I2C_HandleTypeDef* I2cHandle,uint8_t BlockAddr,uint8_t PageAddr,uint8_t BitAddr,uint16_t DataLength,uint8_t* RxData);
void updateTemper(uint16_t NewTemper);
uint16_t readTemper(void);
#endif