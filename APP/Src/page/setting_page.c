#include "page.h"
#include "animation.h"
#include "page/menu_page.h"
#include "page/setting_page.h"
#include "string.h"

static void on_enter_test1()
{
    route_to(&setting_page.page);
}

menu_item setting_page_items[]=
{
    {
        .text = "Return",
        .on_enter = on_enter_test1,
    },
    {
        .text = "Choose Tips",
        .on_enter = on_enter_test1,
    },
    {
        .text = "Erase All Datas",
        .on_enter = on_enter_test1,
    },
    {
        .text = "About",
        .on_enter = on_enter_test1,
    },
    {
        .text = "test5",
        .on_enter = on_enter_test1, 
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

    setting_page.item_lines = (float)(screen_height - header_height) / (float)(item_height);
    setting_page.top_index.set(&setting_page.top_index, 0);
    setting_page.shadow_top.set(&setting_page.shadow_top, header_height);
    setting_page.shadow_bottom.set(&setting_page.shadow_bottom, (float)(item_height + header_height));
    setting_page.shadow_width.set(&setting_page.shadow_width, (float)setting_page.items[0].width);
}