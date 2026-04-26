#include "zedit.h"
#include "../text.h"
#include "../string.h"
#include "../fs/dataset.h"
#include "../arch/i686/keyboard.h"


//ts broken
#define MAX_BUF 4096

static char buffer[MAX_BUF];

static void zedit_readline(char* out, int max)
{
    // it works, trust (not)
    char* line = keyboard_get_line();

    if (!line)
    {
        out[0] = 0;
        return;
    }

    strncpy(out, line, max - 1);
    out[max - 1] = 0;
}

int zedit_run(const char* unused)
{
    char dset[16];
    char fname[16];

    text_print("Available datasets:\n");
    dataset_list();

    text_print("\nSelect dataset: ");
    zedit_readline(dset, sizeof(dset));

    text_print("\nFile name: ");
    zedit_readline(fname, sizeof(fname));

    text_print("\nDEBUG dataset = ");
    text_print(dset);
    text_print("\n");

    text_print("DEBUG file = ");
    text_print(fname);
    text_print("\n");

    if (dset_read_file(dset, fname, (uint8_t*)buffer) < 0)
    {
        text_print("File not found\n");
        return -1;
    }

    text_print("\n=== ZEDIT ===\n");
    text_print(buffer);
    text_print("\n");

    text_print("Editing not implemented yet :3\n");

    return 0;
}
