#include "page/about_page.h"
#include "page/menu_page.h"
#include "page/setting_page.h"
#include "OLEDUI.h"
#include "encoder.h"

void about_page_init(void)
{

}

void about_page_update_ui(void)
{
    about_page *page;
    page = container_of(current_page,about_page,page);//获取当前页面的子类指针

    uint32_t y = lroundf(page->top_y.current_value);
    if(page->top_y.done)
    {
        if(y < 0)
        {
            page->top_y.set(&(page->top_y),0);
        }
        else if(y > 150)
        {
            page->top_y.set(&(page->top_y),150);
        }
    }

    static const char *line[] = {"Author:",      "Hardware", "Hardware", "Hardware", "Hardware", "Hardware", "Hardware", "Hardware",

    };

    uint16_t n_lines = sizeof(line)/sizeof(line[0]);

    for(uint16_t i = 0; i < n_lines; i++)
    {
        DrawString(5,28 + i *14 - y,line[i],2);
    }

    flash();

    page->top_y.update(&(page->top_y));

}
void about_page_enter(void)
{
    about_page *page;
    page = container_of(current_page,about_page,page);//获取当前页面的子类指针
    page->top_y.set(&(page->top_y),0);
}

void about_page_exit(void)
{

}

void about_page_on_encoder_changed(int32_t diff)
{
    about_page *page;
    page = container_of(current_page,about_page,page);//获取当前页面的子类指针
    float end_value = page->top_y.start_value + page->top_y.value_span + (float)diff * 16.0f;
    page->top_y.transition(&(page->top_y),end_value,520);
}

static void about_page_on_key_enter_pressed()
{
    route_to(&(setting_page.page));
}

about_page about_page_instance=
{
    .page.page = about_page_init,
    .page.update_ui = about_page_update_ui,
    .page.enter = about_page_enter,
    .page.exit = about_page_exit,
    .page.on_encoder_changed = about_page_on_encoder_changed,
    .page.key_handlers[ENTER].on_short_pressed = about_page_on_key_enter_pressed,

    .top_y.update = animation_update,
    .top_y.transition = animation_transition,
    .top_y.set = animation_set,
};