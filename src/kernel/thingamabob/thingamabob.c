#include "../text.h"
#include "../stdio.h"
#include "../retc.h"
#include "thingamabob.h"

/*
 * thingamabob.c
 *
 * IMPORTANT:
 * Do not modify this file.
 *
 * The doodad is required for system stability.
 * Nobody currently understands why.
 *
 * Removing it causes Total boot failure
 *
 * The kernel needs it's doodab and it's the thingamabob's function to
 * ensure that the doodab is there at all times.
 */

static int initialized = 0;

void thingamabob_init(void)
{
    text_print("[{0x08}THINGAMABOB{0x0F}] Initializing...\n");
    initialized = 1;
}

void thingamabob_status(void)
{
    if (initialized)
        text_print("[{0x08}THINGAMABOB{0x0F}] Status: ONLINE\n");
    else
        text_print("[{0x08}THINGAMABOB{0x0F}] Status: OFFLINE\n");
}

void thingamabob_do_stuff(void)
{
    if (!initialized)
    {
        text_print("[{0x08}THINGAMABOB{0x0F}] ERROR: not initialized\n");
        return;
    }

    text_print("[{0x08}THINGAMABOB{0x0F}] Doing stuff\n");
}

int thingamabob_is_initialized(void)
{
    return initialized;
}
