#include "process.h"
#include "../heap.h"

static int next_pid = 1;

process_t* create_process()
{
    process_t* p = kmalloc(sizeof(process_t));

    p->pid = next_pid++;
    p->state = PROC_READY;

    p->page_dir = create_page_directory();

    p->next = 0;

    return p;
}
