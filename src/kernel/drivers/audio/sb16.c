#include "sb16.h"
#include "dma.h"
#include "../../text.h"
#include "../../arch/i686/io.h"

#define NOTE_E5 659
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_A4 440
#define NOTE_G4 392

typedef struct {
    uint16_t freq;
    uint16_t duration_ms;
} note_t;

note_t tetris[] = {
    {659, 200}, // E
    {494, 100}, // B
    {523, 100}, // C
    {587, 200}, // D
    {523, 100}, // C
    {494, 100}, // B
    {440, 200}, // A
};

#define DSP_RESET   0x226
#define DSP_READ    0x22A
#define DSP_WRITE   0x22C
#define DSP_STATUS  0x22E

static uint8_t audio_buffer[4096];

static void dsp_wait_write()
{
    while (i686_inb(DSP_WRITE) & 0x80);
}

static void dsp_write(uint8_t v)
{
    dsp_wait_write();
    i686_outb(DSP_WRITE, v);
}

static uint8_t dsp_read()
{
    while (!(i686_inb(DSP_STATUS) & 0x80));
    return i686_inb(DSP_READ);
}

int sb16_init()
{
    text_print("[{0x08}SB16{0x0F}]        Reset...\n");

    i686_outb(DSP_RESET, 1);
    for (volatile int i = 0; i < 10000; i++);
    i686_outb(DSP_RESET, 0);

    if (dsp_read() != 0xAA)
    {
        text_print("[{0x08}SB16{0x0F}]        FAIL\n");
        return 0;
    }

    text_print("[{0x08}SB16{0x0F}]        OK\n");
    return 1;
}

void sb16_play(uint8_t* buf, uint32_t len, uint16_t hz)
{
    if (len < 2) return;

    text_print("[{0x08}SB16{0x0F}]        play start\n");

    // 1. set sample rate
    dsp_write(0x41);
    dsp_write((hz >> 8) & 0xFF);
    dsp_write(hz & 0xFF);

    // 2. stop speaker
    dsp_write(0xD3);

    // 3. DMA first
    dma_set_channel_1(buf, len);

    // 4. speaker on
    dsp_write(0xD1);

    // 5. start playback
    dsp_write(0xC0);
    dsp_write(0x00); // mono unsigned

    // 6. length
    dsp_write((len - 1) & 0xFF);
    dsp_write((len - 1) >> 8);

    text_print("[{0x08}SB16{0x0F}]        triggered\n");
}


void gen_tone(uint8_t* buf, int size, int freq)
{
    int period = 22050 / freq;

    for (int i = 0; i < size; i++)
    {
        buf[i] = ((i / (period / 2)) % 2) ? 0xFF : 0x00;
    }
}
void play_tetris_theme()
{
    uint8_t buffer[4096];

    for (int i = 0; i < sizeof(tetris)/sizeof(note_t); i++)
    {
        note_t n = tetris[i];

        if (n.freq == 0)
        {
            // rest
            for (volatile int d = 0; d < n.duration_ms * 10000; d++);
            continue;
        }

        gen_tone(buffer, sizeof(buffer), n.freq);

        sb16_play(buffer, sizeof(buffer), 22050);

        // crude timing delay
        for (volatile int d = 0; d < n.duration_ms * 100000; d++);
    }
}

void sb16_test_tone()
{
    for (int i = 0; i < sizeof(audio_buffer); i++)
    {
        audio_buffer[i] = (i % 32 < 16) ? 0xFF : 0x00;
    }

    sb16_play(audio_buffer, sizeof(audio_buffer), 22050);
}
