#ifndef IRON_PAGE_H
#define IRON_PAGE_H

#include "page.h"
 
extern page_base iron_page;

void iron_page_init(void);
void iron_page_update_ui(void);
void iron_page_enter(void);
void iron_page_exit(void);
void iron_page_on_encoder_changed(int32_t diff);

#endif /* IRON_PAGE_H */
