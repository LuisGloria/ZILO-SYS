#include "dsview.h"
#include "../text.h"
#include "../fs/dataset.h"

#define COLOR_TOPBAR 0x70
#define COLOR_MAIN   0x1F

static void draw_topbar()
{
    text_set_color(COLOR_TOPBAR);

    for (int i = 0; i < 72; i++)
        text_putc(' ');

    text_set_color(0x70);
    text_print("DATASETS");

    text_set_color(COLOR_MAIN);
    text_print("\n");
}

static void draw_bar(uint32_t size)
{
    int max = 20;
    int blocks = (size > max) ? max : (size * max / 10);

    for (int i = 0; i < max; i++)
        text_putc(i < blocks ? 0xDB : ' ');
}

void dsviewer_run()
{
    text_clear();

    draw_topbar();

    int count = dataset_count();

    text_set_color(COLOR_MAIN);

    for (int i = 0; i < count; i++)
    {
        dataset_t* d = dataset_get(i);
        if (!d) continue;

        text_print(d->name);
        text_print(" | LBA: ");
        text_print_dec(d->start_lba);

        text_print(" | SZ: ");
        text_print_dec(d->size);

        text_print(" | ");

        draw_bar(d->size);

        text_print("\n");
    }

    text_print("\n[Press any key to exit]\n");
    text_set_color(0x0F);
}
