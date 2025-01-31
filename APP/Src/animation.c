#include "animation.h"

inline float map(float t)
{
    //return (t*2.0f / (1.0f + fabsf(t - 0.5f) * 2.0f));
    return 1.0f - (1.0f - t) * (1.0f - t);
}

void animation_update(animation *ani)
{
    if(ani->done){return;}
    ani->current_tick = HAL_GetTick();
    float t = (float)(ani->current_tick - ani->start_tick) / (float)(ani->duration_tick);
    if(ani->current_tick >= ani->start_tick + ani->duration_tick)
    {
        t = 1;
        ani->done = true;
    }
    ani->current_value = ani->start_value + ani->value_span * map(t);
}

void animation_begin(animation *ani, float start_value, float end_value, uint32_t duration_tick)
{
    ani->current_value = start_value;
    ani->start_value = start_value;
    ani->value_span = end_value - start_value;
    ani->duration_tick = duration_tick;
    ani->start_tick = HAL_GetTick();
    ani->done = false;
}

void animation_set(animation *ani,float value)
{
    ani->current_value = value;
    ani->start_value = value;
    ani->value_span = 0;
    ani->duration_tick = 0;
    ani->start_tick = HAL_GetTick();
    ani->done = true;
}

void animation_transition(animation *ani, float end_value, uint32_t duration_tick)
{
    ani->start_value = ani->current_value;
    ani->value_span = end_value - ani->current_value;
    ani->duration_tick = duration_tick;
    ani->start_tick = HAL_GetTick();
    ani->done = false;
}
