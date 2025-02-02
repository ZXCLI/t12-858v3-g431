#ifndef CHOOSE_TIPS_PAGE_H
#define CHOOSE_TIPS_PAGE_H

#include "page.h"
#include "page/menu_page.h"

extern menu_page choose_tips_page_instance;

void choose_tips_page_init(void);
void choose_tips_page_update_ui(void);
void choose_tips_page_enter(void);
void choose_tips_page_exit(void);
void choose_tips_page_on_encoder_changed(int32_t diff);


#endif /* _CHOOSE_TIPS_PAGE_H_ */
