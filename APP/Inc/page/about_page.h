#ifndef ABOUT_PAGE_H
#define ABOUT_PAGE_H

#include "page.h"  
#include "animation.h" 

typedef struct 
{
    page_base page;
    animation top_y;
    uint16_t last_y;
}about_page;

extern about_page about_page_instance;

void about_page_init(void);
void about_page_enter(void);
void about_page_exit(void);
void about_page_on_encoder_changed(int32_t diff);

#endif /* ABOUT_PAGE_H */
