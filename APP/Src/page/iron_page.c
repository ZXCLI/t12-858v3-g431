#include "page/iron_page.h"
#include "loop.h"
#include "default.h"

void iron_page_init(void)
{

}

void iron_page_update_ui(void)
{
    if(Iron.AdcReady)
    {

        
    }
}
void iron_page_enter(void)
{

}

void iron_page_exit(void)
{
    IRON_PWM = 0;
    FAN_PWM = 0;
    Iron.start = 0; //停止加热
    Gun.start = 0; //停止加热
}

void iron_page_on_encoder_changed(int32_t diff)
{

}

page_base iron_page=
{
    .page = iron_page_init,
    .update_ui = iron_page_update_ui,
    .enter = iron_page_enter,
    .exit = iron_page_exit,
    .on_encoder_changed = iron_page_on_encoder_changed,
};
