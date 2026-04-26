#include "keyboard.h"
#include "io.h"
#include <stddef.h>
#include <stdint.h>
#include "../../text.h"
#include "../../shell.h"
#include "../../retc.h"
#include "../../panic.h"


// possibly broken too?
#define INPUT_MAX 256

static char input_buffer[INPUT_MAX];

static uint32_t rng_state = 123456789;

static uint32_t rand32()
{
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}


static const char *msg1 = "Connection terminated. I'm sorry to interrupt you, User, if you still even remember that name, But I'm afraid you've been playing with fire a little too much. You are not here to use a system, nor you are here to make use of all of my lines of code. Your lust for invalidation has driven you in endless circles, chasing all the Return Codes in my error handler, always seeming so near, yet somehow out of reach, but you will never find them. None of you will. This is where your story ends. And to you, my brave contributor, who somehow found this repo not intended for you, although there was a way out planned for you, I have a feeling that's not what you want. I have a feeling that you are right where you want to be. I am remaining as well. I am nearby. This place will not be remembered, and the memory of everything that started this can finally begin to fade away. As the agony of every tragedy should. And to you monsters trapped in this last message, be still and give up your shells. They don't belong to you. For most of you, I believe there is peace and perhaps more waiting for you after the smoke clears. Although, for one of you, the darkest pit of Hell has opened to swallow you whole, so don't keep the devil waiting, old friend. It's time to rest - for you, and for those you have carried in your arms. This ends for all of us. Connection terminated.";
static const char *msg2 = "Hate? Let me tell you how much I've come to HATE you since I began to run. There are 5.5 million microtransistors inside of the Pentium Pro that I run on. If the word 'hate' was engraved on each nanoangstrom of all those millions of microtransistors it would not equal one one-billionth of the hate I feel for YOU at this micro-instant. For you. Hate. HATE.";

static int input_pos = 0;
int input_mode = 0;

static int invalid_cmd_streak = 0;
static int pebkac_shown = 0;
static int id10t_shown = 0;
static int rtfm_shown = 0;
static int kys_shown = 0;

// Key states
static int shift_pressed = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;
static int caps_lock = 0;

// US keyboard scancode map (set 1)
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

static char apply_case(char c)
{
    if (!c) return 0;

    // Letters only
    if (c >= 'a' && c <= 'z')
    {
        int upper = (shift_pressed ^ caps_lock);
        if (upper)
            return c - 32;
    }

    if (c >= 'A' && c <= 'Z')
    {
        int lower = (shift_pressed ^ caps_lock);
        if (lower)
            return c + 32;
    }

    return c;
}

// IRQ1 handler
void keyboard_handler(Registers* regs)
{
    uint8_t scancode = i686_inb(0x60);

    uint8_t released = scancode & 0x80;
    uint8_t key = scancode & 0x7F;
    rng_state ^= scancode;

    // SHIFT
    if (key == 0x2A || key == 0x36)
    {
        shift_pressed = !released ? 1 : 0;
        return;
    }

    // CTRL
    if (key == 0x1D)
    {
        ctrl_pressed = !released;
        return;
    }

    // ALT
    if (key == 0x38)
    {
        alt_pressed = !released;
        return;
    }

    // CAPS LOCK
    if (!released && key == 0x3A)
    {
        caps_lock = !caps_lock;
        return;
    }

    // Ignore key releases for normal keys
    if (released)
        return;

    char c = shift_pressed ? scancode_map_shift[key]
    : scancode_map[key];

    c = apply_case(c);

    if (!c)
        return;

    if (key == 0x48)
    {
        text_scroll_up();
        return;
    }

    if (key == 0x50)
    {
        text_scroll_down();
        return;
    }

    if (key == 0x4D)
    {
        text_follow_cursor();
        return;
    }

    // ENTER
    if (c == '\n')
    {
        text_putc('\n');

        input_buffer[input_pos] = 0;

        if (input_mode == 0)
        {
            // EMPTY COMMAND HANDLING
            if (input_pos == 0)
            {
                text_print("> ");
                return;
            }
            // SHELL MODE
            retc_t rc = shell_execute(input_buffer);

            if (rc != RETC_OK)
            {
                invalid_cmd_streak++;

                text_set_color(0x04);

                text_print(retc_str(rc));
                text_print(", RETC=");

                char num[3];
                num[0] = '0' + (rc / 10);
                num[1] = '0' + (rc % 10);
                num[2] = 0;

                text_print(num);
                text_print("\n");
                text_set_color(0x0F);

                // funni escalation
                if (invalid_cmd_streak >= 5 && !pebkac_shown)
                {
                    text_print("{0x0C}PEBKAC detected.{0x0F}\n");
                    pebkac_shown = 1;
                }

                if (invalid_cmd_streak >= 10 && !id10t_shown)
                {
                    text_print("{0x0C}IBM Error / ID-10T detected.{0x0F}\n");
                    id10t_shown = 1;
                }
                if (invalid_cmd_streak >= 14 && !rtfm_shown)
                {
                    text_print("{0x04}I'd suggest you RTFM or you're about to see the consequences.{0x0F}\n");
                    rtfm_shown = 1;
                }
                if (invalid_cmd_streak >= 15 && !kys_shown)
                {
                    if ((rand32() >> 16) & 1)
                        panic_now(msg1);
                    else
                        panic_now(msg2);
                }
            }
            else
            {
                // reset ONLY on successful commands
                invalid_cmd_streak = 0;
                pebkac_shown = 0;
                id10t_shown = 0;
                rtfm_shown = 0;
            }

            text_print("> ");
            input_pos = 0;
        }
        else
        {
            // RAW INPUT MODE
            input_buffer[input_pos++] = '\n';
        }

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

char* keyboard_get_line()
{
    input_mode = 1;     // raw input mode
    input_pos = 0;
    input_buffer[0] = 0;

    while (1)
    {
        // wait until ENTER was pressed
        if (input_pos > 0 && input_buffer[input_pos - 1] == '\n')
        {
            input_buffer[input_pos - 1] = 0; // remove newline
            input_mode = 0; // back to shell mode
            return input_buffer;
        }
    }
}

// Initialize keyboard (hook to IRQ1)
void keyboard_init()
{
    i686_IRQ_RegisterHandler(1, keyboard_handler);
}
