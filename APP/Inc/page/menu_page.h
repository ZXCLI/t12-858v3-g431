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

enum is_visible_enum
{
    INVISIBLE = 0,
    VISIBLE = 1,
};

enum check_box_or_pop_enum
{
    CHECK_BOX = 0,
    NUM = 1,
};

enum is_checked_enum
{
    UNCHECKED = 0,
    CHECKED = 1,
};

typedef struct
{
    unsigned int is_visible : 1;//是否显示控件,0不显示，1显示
    unsigned int check_box_or_pop : 1;//复选框或者数字弹出框,0复选框，1数字弹出框
    unsigned int is_checked : 1;//复选框是否选中,0未选中，1选中
    int32_t pop_value;//数字弹出框的值
    void (*on_click_pop)(int32_t diff);//弹出框回调函数
}control;

typedef struct
{
    char *text;//选项名称
    uint16_t width;//选项宽度
    void (*on_enter)(void);//进入选项时调用
    control ctrl;//选项的小控件
}menu_item;
 
typedef struct 
{
    page_base page;//父类
    void (*realy_on_click_pop)(int32_t diff);
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
void menu_page_enter_menu_item(void);
void menu_page_exit(void);
void menu_item_contrl_check_toggle();
void menu_item_contrl_num_change();
void menu_item_on_click_pop(int32_t diff);
uint8_t getDigitCount(int num);
void none(void);
#endif
