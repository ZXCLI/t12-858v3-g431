#include "choose_tips_page.h"

void choose_tips_page_init(void)
{

}

void choose_tips_page_update_ui(void)
{

}
void choose_tips_page_enter(void)
{

}

void choose_tips_page_exit(void)
{
   
}

void choose_tips_page_on_encoder_changed(int32_t diff)
{

}

menu_page choose_tips_page_instance=
{
    .page.page = choose_tips_page_init,
    .page.update_ui = choose_tips_page_update_ui,
    .page.enter = choose_tips_page_enter,
    .page.exit = choose_tips_page_exit,
    .page.on_encoder_changed = choose_tips_page_on_encoder_changed,
};