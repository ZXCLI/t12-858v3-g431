#include "page/menu_page.h"
#include "OLEDUI.h"

void menu_page_update_ui()
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    page->top_index.update(&(page->top_index));
    page->shadow_top.update(&(page->shadow_top));
    page->shadow_bottom.update(&(page->shadow_bottom));
    page->shadow_width.update(&(page->shadow_width));

    int16_t item_bottom = header_height;
    float top_index_value = page->top_index.current_value;
    int16_t rounded_offset_y = lroundf((top_index_value - floorf(top_index_value)) * (float)item_height);
    item_bottom -= rounded_offset_y;
    for(uint16_t i = floor(top_index_value); i < page->item_count; i++)
    {
        DrawString(5, item_bottom + 4, page->items[i].text,item_height >> 4);
        uint8_t temp_bit  = (page->items[i].ctrl.is_visible << 2) | 
                            (page->items[i].ctrl.check_box_or_pop << 1) | 
                            (page->items[i].ctrl.is_checked);
        switch(temp_bit)
        {
            case 0b110:
                DrawNum(110, item_bottom + 4, page->items[i].ctrl.pop_value, 
                        getDigitCount(page->items[i].ctrl.pop_value), 1, 1);
                break;
            case 0b101:
                DrawfillRoundRect(112, item_bottom + 4, 6, 6, 2);//复选框被选中,绘制中心填充
            case 0b100:
                DrawRoundRect(110, item_bottom + 2, 10, 10, 2);//绘制复选框轮廓
                break;
        }

        item_bottom += item_height;
        //DrawLine(2, item_bottom - item_height / 2, )
    }//绘制菜单项
    
    int16_t biased_shadow_top = lroundf(page->shadow_top.current_value - top_index_value * (float)item_height);
    int16_t biased_shadow_bottom = lroundf(page->shadow_bottom.current_value - top_index_value * (float)item_height);
    int16_t shadow_height = biased_shadow_bottom - biased_shadow_top;
    if(biased_shadow_top < 0)
    {
        shadow_height += biased_shadow_top;
        biased_shadow_top = 0;
    }
    if(shadow_height > 0) //&& (page->items[page->current_item].ctrl.on_click_pop == NULL))
    {
        DrawMYfillRoundRect(2, biased_shadow_top, (uint16_t)(page->shadow_width.current_value), 
                            (uint16_t)shadow_height);
    }//绘制选项框阴影

    if(page->items[page->current_item].ctrl.on_click_pop != NULL)
    {
        page->items[page->current_item].ctrl.on_click_pop(0);//绘制弹出框
    }//弹出框回调函数
    flash();
}

//将选项框移动到指定位置
void menu_page_focus_to(uint16_t target_index)
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    if(page->item_count == 0){return;}
    if(target_index >= page->item_count)
    {
        target_index = page->item_count - 1;
    }
    if(page->current_item != target_index)
    {
        float top_value = (float)header_height + ((float)target_index * (float)item_height);
        float bottom_value = top_value + (float)item_height;
        if(page->current_item < target_index)
        {
            //向下滚动
            page->shadow_top.transition(&(page->shadow_top), top_value, 400);
            page->shadow_bottom.transition(&(page->shadow_bottom), bottom_value, 200);
        }
        else
        {
            //向上滚动
            page->shadow_top.transition(&(page->shadow_top), top_value, 200);
            page->shadow_bottom.transition(&(page->shadow_bottom), bottom_value, 400);
        }
        page->shadow_width.transition(&(page->shadow_width), (float)page->items[target_index].width, 200);
    }
    menu_page_ensure_visible(target_index);
    page->current_item = target_index;
}

//以增量形式移动，并确保当前选项可见
void menu_page_foucs_with_increment(int16_t increment)
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    if((increment < 0) && (page->current_item < (-increment)))
    {
        menu_page_focus_to(0);
        return;
    }//运行到第一个选项时，再向上移动时回到第一个选项

    if(page->current_item + increment >= page->item_count)
    {
        menu_page_focus_to(page->item_count - 1);
        return;
    }//运行到最后一个选项时，再向下移动时回到最后一个选项

    menu_page_focus_to(page->current_item + increment);
}

//确保当前选项可见，如果不可见则移动选项
void menu_page_ensure_visible(uint16_t target_index)
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    if(page->item_count == 0){return;}
    if(target_index >= page->item_count)
    {
        target_index = page->item_count - 1;
    }//超出范围则返回最后一个选项,防止越界

    float i_top = page->top_index.start_value + page->top_index.value_span;
    float i_bottom = i_top + page->item_lines;
    float i_target = (float)target_index;
    if(i_top <= i_target && i_target <= i_bottom - 1) {return;}
    if(i_top > i_target)
    {
        page->top_index.transition(&(page->top_index), i_target, 200);
    }
    else
    {
        page->top_index.transition(&(page->top_index), (i_target - page->item_lines + 1), 200);
    }
}

//页面对编码器变化的响应
void menu_page_on_encoder_changed(int32_t diff)
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    if(page->items[page->current_item].ctrl.on_click_pop != NULL)
    {
        //当有弹窗出现时，弹窗需要提前捕获编码器的值
        page->items[page->current_item].ctrl.on_click_pop(diff);
    }
    else
    {
        menu_page_foucs_with_increment(diff);
    }
    
}

//页面对按键的响应函数
void menu_page_enter_menu_item()
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    if((page->current_item < page->item_count) && (page->items[page->current_item].on_enter != NULL))
    {
        page->items[page->current_item].on_enter();//将页面的按键响应传递给选项，执行选项的回调函数
    }
}

void menu_page_exit()
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    page->current_item = 0;
    page->top_index.current_value = 0;
    page->shadow_top.current_value = 0;
}

//带有复选框的菜单项的按键响应函数
void menu_item_contrl_check_toggle()
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    page->items[page->current_item].ctrl.is_checked = !page->items[page->current_item].ctrl.is_checked;
}
//带有弹窗的菜单项的按键响应函数
void menu_item_contrl_num_change()
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针
//每次按下按键，弹窗的函数指针会在NULL和真实的回调函数之间切换
    if(page->items[page->current_item].ctrl.on_click_pop == NULL)
    {
        page->items[page->current_item].ctrl.on_click_pop = page->realy_on_click_pop;
    }
    else
    {
        page->items[page->current_item].ctrl.on_click_pop = NULL;
    }
}
//弹窗对编码器变化的响应函数
void menu_item_on_click_pop(int32_t diff)
{
    menu_page *page;
    page = container_of(current_page,menu_page,page);//获取当前页面的子类指针

    int32_t temp_value = page->items[page->current_item].ctrl.pop_value + diff;
    temp_value = (temp_value > page->items[page->current_item].ctrl.max) ? 
                 page->items[page->current_item].ctrl.max : temp_value;//如果超出范围则返回最大值
    temp_value = (temp_value < page->items[page->current_item].ctrl.min) ?
                 page->items[page->current_item].ctrl.min : temp_value;//如果超出范围则返回最小值
    page->items[page->current_item].ctrl.pop_value = temp_value;
   
    MY_FillRect(15,16,128-50,32);
    DrawRect1(15,16,128-25,32+16);
    DrawString(23,23,page->items[page->current_item].text,1);
    DrawRect1(20,35,20+57,35+8);
    DrawNum(27+50+4,36, page->items[page->current_item].ctrl.pop_value, 
            getDigitCount(page->items[page->current_item].ctrl.pop_value), 1, 1);
    uint16_t diff_value_max = page->items[page->current_item].ctrl.max - page->items[page->current_item].ctrl.min;
    uint16_t diff_value = page->items[page->current_item].ctrl.pop_value - page->items[page->current_item].ctrl.min;
    uint16_t diff_value_width = (uint16_t)((float)(diff_value * 57.0f) / (float)diff_value_max);//计算进度条长度
    FillRect(20,35, diff_value_width, 8);
    
}

uint8_t getDigitCount(int num) 
{
    int count = 0;
    
    // 处理负数
    if (num < 0) {
        num = -num;
    }
    if (num == 0) {
        return 1;
    }
    while (num > 0) {
        num /= 10; // 每次除以10，去掉最低位
        count++;
    }
    return count;
}
void none(void)
{
    
}
