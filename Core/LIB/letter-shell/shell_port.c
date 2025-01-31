/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "shell.h"
#include "shell_port.h"
// #include "usbd_cdc_if.h"
#include "main.h"
#include "string.h"

Shell shell;
char shellBuffer[512];
char RXdata[512];
bool RXready;
bool TXready;
bool TXfull;

uint8_t TXbuf[512] = {};
uint16_t CHARcount = 0;

uint32_t TXtimeout = 0;

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    TXtimeout = HAL_GetTick();

    memcpy(&TXbuf[CHARcount], data, len);//只合并数据，并不发送
    CHARcount += len;

    TXfull = true;

    return len;
}

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    shell.write = userShellWrite;
    shellInit(&shell, shellBuffer, 512);
}

void USRshellRUN(void)
{

    if(HAL_GetTick() - TXtimeout > 10){TXready = true;}//距离最后一次合并数据已经过去10ms，开始发送
    //先接收和处理数据，再进行发送
    if(RXready){
        for(int i = 0; i < 64; i++){
            if(RXdata[i] == '\0'){
                break;
            }
            shellHandler(&shell, RXdata[i]);
            RXdata[i] = 0;//清空接收缓冲区
        }
        RXready = 0;
    }

    if(TXready&&TXfull){
        //CDC_Transmit_FS(TXbuf, CHARcount);//cdc串口发送
        CHARcount = 0;
        memset(TXbuf, 0, sizeof(TXbuf));//清空发送缓冲区
        TXready = false;
        TXfull = false;
    }
}

