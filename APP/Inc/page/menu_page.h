#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include "page.h"
#include "animation.h"
#include "stddef.h"
#include "math.h"

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define item_height 16
#define header_height 0

typedef struct
{
    char *text;//选项名称
    uint16_t width;//选项宽度
    void (*on_enter)(void);//进入选项时调用
}menu_item;
 

typedef struct 
{
    page_base page;//父类
    menu_item *items;//选项
    uint16_t item_count;//选项数量
    uint16_t current_item;//当前选项索引
    animation top_index;//顶部选项索引动画
    animation shadow_top;//复选顶部框阴影动画
    animation shadow_bottom;//复选框底部阴影动画
    animation shadow_width;//复选框阴影宽度动画
    float item_lines;
}menu_page;

void menu_page_update_ui(void);
void menu_page_ensure_visible(uint16_t target_index);
void menu_page_on_encoder_changed(int32_t diff);
void none(void);
#endif
