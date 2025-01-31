#include "shell.h"
#include "shell_port.h"
#include "stdio.h"
// #include "usbd_cdc_if.h"

int func(int argc, char *agrv[])
{
    printf("%dparameter(s)\r\n", argc);
    for (char i = 1; i < argc; i++)
    {
        printf("%s\r\n", agrv[i]);
    }
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), func, func, for test);


int fputc(int ch, FILE *f)
 
{
    //CDC_Transmit_FS((uint8_t *)(&ch), 1);//????
    return ch;
 
}