#ifndef ANIMATION_H
#define ANIMATION_H

#include "main.h"
#include "stdbool.h"
#include "math.h"

// #define map(x) 1 - (1 - x) * (1 - x)
// #define map(x) (float)x*2.0f / (1.0f + fabsf(x - 0.5f) * 2.0f)

typedef struct animation animation; 

struct animation {
    uint32_t start_tick;          // 动画开始时间
    uint32_t current_tick;        // 当前时间
    float current_value;          // 当前值
    float start_value;            // 起始值
    float value_span;             // 动画值变化的跨度
    uint32_t duration_tick;       // 动画持续时长（毫秒）
    bool done;                    // 是否完成
    void (*update)(animation *ani);  // 更新函数
    void (*set)(animation *ani, float value);  // 设置值函数
    void (*transition)(animation *ani, float end_value, uint32_t duration_tick);  // 过渡函数
    void (*begin)(animation *ani, float start_value, float end_value, uint32_t duration_tick);  // 开始函数
};

float map(float t);
void animation_update(animation *ani);
void animation_begin(animation *ani, float start_value, float end_value, uint32_t duration_tick);
void animation_set(animation *ani,float value);
void animation_transition(animation *ani, float end_value, uint32_t duration_tick);

#endif /* ANIMATION_H */
