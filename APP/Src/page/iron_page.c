#include "page/iron_page.h"
#include "loop.h"
#include "default.h"

void iron_page_init(void)
{

}

void iron_page_update_ui(void)
{
    if((Iron.Ready||Gun.Ready)){
        if(Iron.start){
            DrawString(80,17,"ON",2);
        }else{
            DrawString(80,17,"OFF",2);
        }
        Iron.Temp_set = Iron.Temp_Ref_pu * 800;
        if(Iron.Connected){
            if((abs(Iron.Temp_really - Iron.Temp_set)<3)&&(Iron.start)){
                DrawNum(3,19,Iron.Temp_set,3,4,1); 
            }else{
                DrawNum(3,19,Iron.Temp_really,3,4,1); 
            } 
            //DrawMYfillRect(1,52,temp1-2,11);                                          //温度进度条
        }else
        {
            DrawString(3,19,"ERR",4);
            Iron.start = false;
        }

        DrawHalfRoundRect(0,16,74,32+1,0);                      //温度外面的框
        DrawHalfRoundRect(0,51,74,12,0);                          //设定温度外面的框
        //DrawHalfRoundRect(temp+1,51,90-1-temp,12,1);                //实际加热温度外面的框
        DrawHalfRoundRect(78,51,36,12,1);                       //占空比外面的框

        flash();
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

static void iron_page_on_long()
{
    route_to(&(setting_page.page)); //进入设置页面
}

void iron_page_on_enter()
{
    Iron.start = !Iron.start;
}
page_base iron_page=
{
    .page = iron_page_init,
    .update_ui = iron_page_update_ui,
    .enter = iron_page_enter,
    .exit = iron_page_exit,
    .on_encoder_changed = iron_page_on_encoder_changed,
    .key_handlers[ENTER].on_long_pressed = iron_page_on_long,
    .key_handlers[ENTER].on_short_pressed = iron_page_on_enter,
};
