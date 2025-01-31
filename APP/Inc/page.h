#ifndef PAGE_H
#define PAGE_H

#include "main.h"
#include "encoder.h"

#define screen_height 64

typedef struct
{
    void (*page)(void);//进入页面才执行一次该函数，配置UI和指针
    void (*update_ui)(void);//更新UI
    void (*enter)(void);//进入页面执行的函数，配置外设
    void (*exit)(void);//退出页面执行的函数，配置外设
    void (*tick)(void);
    void (*on_encoder_changed)(int32_t diff);//编码器旋转事件
    void (*key_handle[KEY_COUNT])(void);//按键事件
}page_base;

extern page_base *current_page;
void route_to(page_base *new_page);

#endif
