#include "keyboard.h"
#include "io.h"
#include <stddef.h>
#include <stdint.h>
#include "../../text.h"   // adjust if needed
#include "../../shell.h"
#include "../../retc.h"

#define INPUT_MAX 256

static char input_buffer[INPUT_MAX];
static int input_pos = 0;
static int shift_pressed = 0;

// Simple US keyboard scancode map (set 1)
static const char scancode_map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,
    '\\','z','x','c','v','b','n','m',',','.','/',
    0,
    '*',
    0,
    ' ',
};

static const char scancode_map_shift[128] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,
    'A','S','D','F','G','H','J','K','L',':','"','~',
    0,
    '|','Z','X','C','V','B','N','M','<','>','?',
    0,
    '*',
    0,
    ' ',
};

// IRQ1 handler
void keyboard_handler(Registers* regs)
{
    uint8_t scancode = i686_inb(0x60);

    // SHIFT handling (keep your code here)

    if (scancode & 0x80)
        return;

    char c = shift_pressed ? scancode_map_shift[scancode]
    : scancode_map[scancode];

    if (!c)
        return;

    // ENTER
    if (c == '\n')
    {
        text_putc('\n');

        input_buffer[input_pos] = 0;

        retc_t rc = shell_execute(input_buffer);

        if (rc != RETC_OK)
        {
            text_set_color(0x04); // red

            text_print(retc_str(rc));
            text_print(", RETC=");

            char num[3];
            num[0] = '0' + (rc / 10);
            num[1] = '0' + (rc % 10);
            num[2] = 0;

            text_print(num);
            text_print("\n");

            text_set_color(0x0F); // reset to white
        }

        input_pos = 0;

        text_print("> ");
        return;
    }

    // BACKSPACE
    if (c == '\b')
    {
        if (input_pos > 0)
        {
            input_pos--;
            text_putc('\b');
        }
        return;
    }

    // NORMAL CHAR
    if (input_pos < INPUT_MAX - 1)
    {
        input_buffer[input_pos++] = c;
        text_putc(c);
    }
}

// Initialize keyboard (hook IRQ1)
void keyboard_init()
{
    i686_IRQ_RegisterHandler(1, keyboard_handler);
}
