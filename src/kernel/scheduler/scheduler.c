#include "scheduler.h"
#include "../text.h"

process_t* current = 0;
process_t* head = 0;

void scheduler_init()
{
    text_print("[{0x08}SCHEDULER{0x0F}]   Scheduler Initialized\n");
    current = 0;
    head = 0;
}

void add_process(process_t* p)
{
    if (!head)
    {
        head = p;
        p->next = p;
        return;
    }

    process_t* tmp = head;
    while (tmp->next != head)
        tmp = tmp->next;

    tmp->next = p;
    p->next = head;
}

void schedule()
{
    if (!current)
    {
        current = head;
        return;
    }

    current = current->next;

    switch_page_directory(current->page_dir);

    // TODO: add context switch registers here
}
