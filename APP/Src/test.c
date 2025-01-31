// #include <stdint.h>
// #include <stdbool.h>
// #include <math.h>
// #include <stdio.h>

// // 假设 HAL_GetTick 返回系统当前的毫秒数（tick）
// extern uint32_t HAL_GetTick(void);

// // 定义一些常量和类型
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define HEADER_HEIGHT 16
// #define ITEM_HEIGHT 12

// // 结构体定义
// typedef struct {
//     const char *text;
//     void (*on_enter)(void);  // 选择该项时调用的函数
//     uint32_t width;          // 项目文本的宽度
// } menu_item;

// typedef struct {
//     float current_value;    // 当前值
//     float start_value;      // 动画开始时的起始值
//     float value_span;       // 动画过渡的跨度（目标值 - 当前值）
//     uint32_t duration_tick; // 动画持续时间（单位：tick）
//     uint32_t start_tick;    // 动画开始的时间（单位：tick）
//     bool done;              // 动画是否完成
// } animation;

// // menu_page 结构体定义
// typedef struct {
//     const char *title;
//     menu_item *items;
//     size_t item_count;
//     size_t current;
//     animation top_index;
//     animation shadow_top;
//     animation shadow_bottom;
//     animation shadow_width;
//     uint32_t font_height;
//     uint32_t item_height;
//     uint32_t item_lines;
//     uint32_t header_height;
//     void (*key_handlers[4])(void);  // 假设有 4 个按键处理函数
// } menu_page;

// // 假设的屏幕和函数定义
// void u8g2_SetFont(void *screen, const char *font) { }
// void u8g2_ClearBuffer(void *screen) { }
// void u8g2_SetClipWindow(void *screen, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) { }
// void u8g2_DrawUTF8(void *screen, uint32_t x, uint32_t y, const char *text) { }
// void u8g2_DrawLine(void *screen, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) { }
// void u8g2_SetDrawColor(void *screen, uint32_t color) { }
// void u8g2_DrawRBox(void *screen, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t radius) { }
// void u8g2_SetMaxClipWindow(void *screen) { }
// void u8g2_SendBuffer(void *screen) { }
// uint32_t u8g2_GetUTF8Width(void *screen, const char *text) { return 50; }  // 假定每个文本的宽度为 50

// // // HAL_GetTick 模拟
// // uint32_t HAL_GetTick(void) {
// //     return 0;  // 简化，返回一个固定的时间
// // }

// // 过渡动画函数
// void animation_transition(animation *anim, float end_value, uint32_t duration_tick) {
//     anim->start_value = anim->current_value;
//     anim->value_span = end_value - anim->current_value;
//     anim->duration_tick = duration_tick;
//     anim->start_tick = HAL_GetTick();
//     anim->done = false;
// }

// // menu_page 函数定义

// void menu_page_update_ui(menu_page *page) {
//     // 更新动画
//     animation_transition(&page->top_index, page->top_index.current_value, 200);
//     animation_transition(&page->shadow_top, page->shadow_top.current_value, 200);
//     animation_transition(&page->shadow_bottom, page->shadow_bottom.current_value, 200);
//     animation_transition(&page->shadow_width, page->shadow_width.current_value, 200);

//     u8g2_SetFont(NULL, "6x12");

//     // 清屏
//     u8g2_ClearBuffer(NULL);

//     // 绘制标题
//     u8g2_SetClipWindow(NULL, 0, 0, SCREEN_WIDTH, HEADER_HEIGHT);
//     u8g2_DrawUTF8(NULL, 0, HEADER_HEIGHT - 1, page->title);

//     // 绘制内容
//     u8g2_SetClipWindow(NULL, 0, HEADER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
//     uint32_t item_bottom = HEADER_HEIGHT;
//     float top_index_value = page->top_index.current_value;
//     uint32_t rounded_offset_y = (uint32_t)roundf((top_index_value - floorf(top_index_value)) * (float)ITEM_HEIGHT);
//     item_bottom -= rounded_offset_y;
//     for (size_t i = (size_t)floorf(top_index_value); i < page->item_count; i++) {
//         item_bottom += ITEM_HEIGHT;
//         u8g2_DrawLine(NULL, 2, item_bottom - ITEM_HEIGHT / 2, 4, item_bottom - ITEM_HEIGHT / 2);
//         u8g2_DrawUTF8(NULL, 10, item_bottom - 1, page->items[i].text);
//     }

//     u8g2_SetDrawColor(NULL, 2);
//     int32_t biased_shadow_top = (int32_t)roundf(page->shadow_top.current_value - top_index_value * (float)ITEM_HEIGHT);
//     int32_t biased_shadow_bottom = (int32_t)roundf(page->shadow_bottom.current_value - top_index_value * (float)ITEM_HEIGHT);
//     int32_t shadow_height = biased_shadow_bottom - biased_shadow_top;
//     if (biased_shadow_top < 0) {
//         shadow_height += biased_shadow_top;
//         biased_shadow_top = 0;
//     }
//     if (shadow_height > 0) {
//         u8g2_SetDrawColor(NULL, 2);
//         u8g2_DrawRBox(NULL, 0, (uint32_t)biased_shadow_top, (uint32_t)page->shadow_width.current_value,
//                       (uint32_t)shadow_height, 2);
//         u8g2_SetDrawColor(NULL, 1);
//     }

//     u8g2_SetMaxClipWindow(NULL);
//     u8g2_SendBuffer(NULL);
// }

// void menu_page_focus_to(menu_page *page, size_t target_index) {
//     if (target_index >= page->item_count) {
//         target_index = page->item_count - 1;
//     }
//     if (page->current != target_index) {
//         float top_value = (float)HEADER_HEIGHT + (float)target_index * (float)ITEM_HEIGHT;
//         float bottom_value = top_value + (float)ITEM_HEIGHT;
//         if (page->current < target_index) {
//             animation_transition(&page->shadow_top, top_value, 400);
//             animation_transition(&page->shadow_bottom, bottom_value, 200);
//         } else {
//             animation_transition(&page->shadow_top, top_value, 200);
//             animation_transition(&page->shadow_bottom, bottom_value, 400);
//         }
//         animation_transition(&page->shadow_width, (float)page->items[target_index].width + 14, 200);
//     }
//     page->current = target_index;
// }

// void menu_page_focus_with_increment(menu_page *page, int increment) {
//     if (increment < 0 && page->current < (-increment)) {
//         return menu_page_focus_to(page, 0);
//     }
//     if (page->current + increment >= page->item_count) {
//         return menu_page_focus_to(page, page->item_count - 1);
//     }
//     return menu_page_focus_to(page, page->current + increment);
// }

// void menu_page_ensure_visible(menu_page *page, size_t target_index) {
//     if (target_index >= page->item_count) {
//         target_index = page->item_count - 1;
//     }
//     float i_top = (float)page->top_index.current_value;
//     float i_bottom = i_top + (float)page->item_lines;
//     float i_target = (float)target_index;
//     if (i_top <= i_target && i_target <= i_bottom - 1) {
//         return;
//     }
//     if (i_top > i_target) {
//         animation_transition(&page->top_index, i_target, 200);
//     } else {
//         animation_transition(&page->top_index, i_target - page->item_lines + 1, 200);
//     }
// }

// void menu_page_on_encoder_changed(menu_page *page, int32_t diff) {
//     menu_page_focus_with_increment(page, diff);
// }

// void menu_page_enter_menu_item(menu_page *page) {
//     if (page->current < page->item_count) {
//         page->items[page->current].on_enter();
//     }
// }

// // menu_page 构造函数
// void menu_page_init(menu_page *page, const char *title, menu_item *items, size_t item_count) {
//     page->title = title;
//     page->items = items;
//     page->item_count = item_count;
//     page->current = 0;

//     for (int i = 0; i < item_count; ++i) {
//         page->items[i].width = u8g2_GetUTF8Width(NULL, page->items[i].text);
//     }

//     page->header_height = ITEM_HEIGHT;
//     page->item_height = ITEM_HEIGHT;
//     page->item_lines = (SCREEN_HEIGHT - HEADER_HEIGHT) / ITEM_HEIGHT;

//     animation_transition(&page->top_index, 0, 200);
//     animation_transition(&page->shadow_top, 0, 200);
//     animation_transition(&page->shadow_bottom, 0, 200);
//     animation_transition(&page->shadow_width, (float)page->items[0].width + 14, 200);
// }

