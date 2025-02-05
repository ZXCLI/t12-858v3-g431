#include "page.h"
#include "animation.h"
#include "page/menu_page.h"
#include "page/setting_page.h"
#include "page/iron_page.h"
#include "page/choose_tips_page.h"
#include "page/about_page.h"
#include "string.h"

static void on_enter_test1()
{
    route_to(&setting_page.page);
}

static void on_enter_return()
{
    route_to(&iron_page);
}

static void on_enter_choose_tips()
{
    route_to(&choose_tips_page_instance.page);
}

static void on_enter_about()
{
    route_to(&about_page_instance.page);
}

menu_item setting_page_items[]=
{
    {
        .text = "Return",
        .on_enter = on_enter_return,

    },
    {
        .text = "TEST POP",
        .on_enter = menu_item_contrl_num_change,
        .ctrl.is_visible = VISIBLE,
        .ctrl.check_box_or_pop = NUM,
        .ctrl.min = 0,
        .ctrl.max = 100,
    },
    {
        .text = "TEST CHECK",
        .on_enter = menu_item_contrl_check_toggle, 
        .ctrl.is_visible = VISIBLE,
        .ctrl.check_box_or_pop = CHECK_BOX,
        .ctrl.is_checked = CHECKED,
    },
    {
        .text = "About",
        .on_enter = on_enter_about,
    },
    {
        .text = "test5",
        .on_enter = menu_item_contrl_check_toggle, 
        .ctrl.is_visible = VISIBLE,
        .ctrl.check_box_or_pop = CHECK_BOX,
        .ctrl.is_checked = CHECKED,
    },
    {
        .text = "test6",
        .on_enter = on_enter_test1,
    },
    {
        .text = "test7",
        .on_enter = on_enter_test1,
    },
    {
        .text = "test8",
        .on_enter = on_enter_test1,
    },
};

menu_page setting_page=
{
    .items = setting_page_items,
    .item_count = sizeof(setting_page_items)/sizeof(menu_item),

    .page.page = setting_page_init,
    .page.enter = none,
    .page.update_ui = menu_page_update_ui,
    .page.on_encoder_changed = menu_page_on_encoder_changed,
    .page.key_handlers[ENTER].on_short_pressed = menu_page_enter_menu_item,
    .realy_on_click_pop = menu_item_on_click_pop,


    .top_index.update = animation_update,
    .top_index.transition = animation_transition,
    .top_index.set = animation_set,

    .shadow_top.update = animation_update,
    .shadow_top.transition = animation_transition,
    .shadow_top.set = animation_set,

    .shadow_bottom.update = animation_update,
    .shadow_bottom.transition = animation_transition,
    .shadow_bottom.set = animation_set,

    .shadow_width.update = animation_update,
    .shadow_width.transition = animation_transition,
    .shadow_width.set = animation_set,
};

void setting_page_init()
{
    for(int i = 0; i < setting_page.item_count; ++i)
    {
        setting_page.items[i].width = 6 * strlen(setting_page.items[i].text) + 5;
    }

    //进入界面时自动恢复原位置，
    //第一次初始化在第一个位置，之后再进入就会自动恢复到上次退出该界面的位置
    setting_page.item_lines = (float)(screen_height - header_height) / (float)(item_height);

    setting_page.top_index.set(&setting_page.top_index, 0 + setting_page.top_index.current_value);
    setting_page.shadow_top.set(&setting_page.shadow_top, header_height + setting_page.current_item * item_height);
    setting_page.shadow_bottom.set(&setting_page.shadow_bottom, (float)(item_height + header_height + 
                                                                        setting_page.current_item * item_height));
    setting_page.shadow_width.set(&setting_page.shadow_width, (float)setting_page.items[setting_page.current_item].width);
}