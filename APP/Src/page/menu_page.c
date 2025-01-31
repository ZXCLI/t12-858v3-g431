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
    if(shadow_height > 0)
    {
        DrawMYfillRoundRect(2, biased_shadow_top, (uint16_t)(page->shadow_width.current_value), 
                            (uint16_t)shadow_height);
    }//绘制选项框阴影

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

void menu_page_on_encoder_changed(int32_t diff)
{
    menu_page_foucs_with_increment(diff);
}

void none(void)
{
    
}
