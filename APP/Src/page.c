#include "page.h"

page_base *current_page;

void route_to(page_base *new_page)
{
    if(current_page != NULL)
    {
        current_page->exit();
    }
    current_page = new_page;
    current_page->page();
    current_page->enter();
}