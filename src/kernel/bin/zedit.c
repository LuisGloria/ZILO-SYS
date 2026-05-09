#include "zedit.h"
#include "../text.h"
#include "../string.h"
#include "../fs/zdsfs.h"
#include "../arch/i686/keyboard.h"

#define MAX_BUF 4096

static char buffer[MAX_BUF];
static void zedit_readline(char* out, int max)
{
    char* line = keyboard_get_line();

    if (!line)
    {
        out[0] = 0;
        return;
    }

    strncpy(out, line, max - 1);
    out[max - 1] = 0;
}

static int zedit_read_number()
{
    char buf[8];

    while (1)
    {
        zedit_readline(buf, sizeof(buf));

        if (buf[0] != 0)
            break;
    }

    int n = 0;

    for (int i = 0; buf[i]; i++)
    {
        if (buf[i] >= '0' && buf[i] <= '9')
            n = n * 10 + (buf[i] - '0');
    }

    return n;
}

int zedit_run(const char* unused)
{
    (void)unused;

    text_print("\n=== ZEDIT (ZDSFS EDITOR) ===\n\n");
    text_print("Datasets:\n");

    int count = zdsfs_count();

    for (int i = 0; i < count; i++)
    {
        zdsfs_dscb_t* d = zdsfs_get(i);

        if (!d) continue;

        text_print_dec(i);
        text_print(": ");
        text_print(d->name);
        text_print("\n");
    }

    text_print("\nSelect dataset number: ");
    int idx = zedit_read_number();

    dataset_t* selected = zdsfs_get(idx);

    if (!selected)
    {
        text_print("Invalid selection\n");
        return -1;
    }

    text_print("\nSelected: ");
    text_print(selected->name);
    text_print("\n");

    if (zdsfs_read(selected->name, (uint8_t*)buffer) < 0)
    {
        text_print("Failed to read dataset\n");
        return -1;
    }

    text_print("\n=== CONTENT ===\n");
    text_print(buffer);
    text_print("\n");
    text_print("\nEditing not implemented yet :3\n");

    return 0;
}
