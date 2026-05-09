#include "sb16.h"
#include "dma.h"
#include "../../text.h"
#include "../../arch/i686/io.h"


#define NOTE_D5 587
#define NOTE_A4 440
#define NOTE_C4 262
#define NOTE_B4 494
#define NOTE_E4 330
#define NOTE_E5 659
#define NOTE_G4 392
#define NOTE_C5 523
#define NOTE_A3 220
#define NOTE_F4 349
#define PAD_FREQ 165  // low E-ish drone


#define DSP_RESET   0x226
#define DSP_READ    0x22A
#define DSP_WRITE   0x22C
#define DSP_STATUS  0x22E

typedef struct {
    uint16_t freq;
    uint16_t duration_ms;
} note_t;

typedef struct {
    uint16_t freq;
    uint16_t duration_ms;
} event_t;

typedef enum
{
    INST_SQUARE,
    INST_SOFT,
    INST_BASS,
    INST_ORGAN,
    INST_NOISE
} instrument_t;

event_t aria_seq[] = {
    {NOTE_C4, 714},
    {NOTE_B4, 714},
    {0,       714},  // wait

    {NOTE_E5, 714},
    {NOTE_E4, 714},
    {0,       714},

    {NOTE_B4, 714},
    {0,       714},

    {NOTE_E4, 714},
    {0,       714},

    {NOTE_G4, 714},
    {NOTE_C5, 714},
    {NOTE_B4, 714},

    {0,       714},

    {NOTE_G4, 714},
    {0,       714},
    {0,       714},  // extended rest

    {NOTE_B4, 714},
    {0,       714},

    {NOTE_E5, 714},
    {NOTE_E4, 714},
    {0,       714},

    {NOTE_B4, 714},
    {0,       714},

    {NOTE_E4, 714},
    {0,       714},

    {NOTE_G4, 714},
    {NOTE_C5, 714},
    {NOTE_B4, 714},

    {0,       714},

    {NOTE_G4, 714}
};


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
}


void gen_tone(uint8_t* buf, int size, int freq)
{
    int period = 22050 / freq;

    for (int i = 0; i < size; i++)
    {
        buf[i] = ((i / (period / 2)) % 2) ? 0xFF : 0x00;
    }
}


//simple synth gen, PCM via SB16

void gen_instrument_tone(
    uint8_t* buf,
    int size,
    int freq,
    instrument_t inst
)
{
    if (freq <= 0)
        return;

    float sample_rate = 22050.0f;
    float phase = 0.0f;
    float step = (float)freq / sample_rate;

    int fade_len = size / 4; // 10% fade in/out

    for (int i = 0; i < size; i++)
    {
        float s = 0.0f;

        switch (inst)
        {
            case INST_SQUARE:
                s = (phase < 0.5f) ? 1.0f : -1.0f;
                break;

            case INST_SOFT:
                s =
                (phase < 0.25f) ? (phase * 4.0f) :
                (phase < 0.75f) ? (2.0f - phase * 4.0f) :
                (phase * 4.0f - 4.0f);
                break;

            case INST_BASS:
                s =
                ((phase < 0.5f) ? 1.0f : -1.0f) * 0.7f +
                ((phase < 0.25f) ? 0.3f : -0.3f);
                break;

            case INST_ORGAN:
                s =
                ((phase < 0.5f) ? 0.6f : -0.6f) +
                ((phase < 0.25f) ? 0.3f : -0.3f) +
                ((phase < 0.125f) ? 0.1f : -0.1f);
                break;

            case INST_NOISE:
                s = ((i * freq) % 97 < 48) ? 1.0f : -1.0f;
                break;
        }

        // FADE IN / OUT ENVELOPE
        float env;

        if (i < fade_len)
            env = (float)i / fade_len;
        else
        {
            float t = (float)(i - fade_len) / (size - fade_len);
            env = 1.0f - (t * t); // smoother decay
        }

        s *= env;

        // convert to PCM
        int sample = (int)(128 + s * 60);

        if (sample < 0) sample = 0;
        if (sample > 255) sample = 255;

        buf[i] = (uint8_t)sample;

        phase += step;
        if (phase >= 1.0f)
            phase -= 1.0f;
    }
}

void play_aria_math()
{
    uint8_t buffer_lead[4096];
    uint8_t buffer_pad[4096];
    uint8_t buffer_mix[4096];

    int count = sizeof(aria_seq) / sizeof(event_t);

    for (int i = 0; i < count; i++)
    {
        event_t e = aria_seq[i];

        // rest
        if (e.freq == 0)
        {
            for (volatile int d = 0; d < e.duration_ms * 8000; d++);
            continue;
        }

        // lead
        gen_instrument_tone(
            buffer_lead,
            sizeof(buffer_lead),
                            e.freq,
                            INST_SOFT
        );

        // pad
        gen_instrument_tone(
            buffer_pad,
            sizeof(buffer_pad),
                            PAD_FREQ,
                            INST_ORGAN
        );

        // mix
        for (int j = 0; j < sizeof(buffer_mix); j++)
        {
            int mixed =
            ((int)buffer_lead[j] - 128) +
            ((int)buffer_pad[j] - 128);

            mixed /= 2;

            mixed += 128;

            if (mixed < 0) mixed = 0;
            if (mixed > 255) mixed = 255;

            buffer_mix[j] = (uint8_t)mixed;
        }

        sb16_play(buffer_mix, sizeof(buffer_mix), 22050);

        // timing
        for (volatile int d = 0; d < e.duration_ms * 5000; d++);
    }
}

void play_mac_chime()
{
    uint8_t c4[11025];
    uint8_t g4[11025];
    uint8_t c5[11025];
    uint8_t e5[11025];
    uint8_t mix[11025];

    gen_instrument_tone(c4, sizeof(c4), 262, INST_ORGAN);
    gen_instrument_tone(g4, sizeof(g4), 392, INST_ORGAN);
    gen_instrument_tone(c5, sizeof(c5), 523, INST_ORGAN);
    gen_instrument_tone(e5, sizeof(e5), 659, INST_ORGAN);

    for (int i = 0; i < sizeof(mix); i++)
    {
        int s =
        ((int)c4[i] - 128) +
        ((int)g4[i] - 128) +
        ((int)c5[i] - 128) +
        ((int)e5[i] - 128);

        s /= 4;
        s += 128;

        if (s < 0) s = 0;
        if (s > 255) s = 255;

        mix[i] = (uint8_t)s;
    }

    sb16_play(mix, sizeof(mix), 22050);
}

void play_kernel_panic_chime()
{
    uint8_t n1[11025];
    uint8_t n2[11025];
    uint8_t n3[11025];

    gen_instrument_tone(n1, sizeof(n1), NOTE_E4, INST_ORGAN);
    sb16_play(n1, sizeof(n1), 22050);

    for (volatile int d = 0; d < 3000000; d++);

    gen_instrument_tone(n2, sizeof(n2), NOTE_E4, INST_ORGAN);
    sb16_play(n2, sizeof(n2), 22050);

    for (volatile int d = 0; d < 3000000; d++);

    gen_instrument_tone(n3, sizeof(n3), NOTE_A3, INST_BASS);
    sb16_play(n3, sizeof(n3), 22050);
}

void sb16_test_tone()
{
    for (int i = 0; i < sizeof(audio_buffer); i++)
    {
        audio_buffer[i] = (i % 32 < 16) ? 0xFF : 0x00;
    }

    sb16_play(audio_buffer, sizeof(audio_buffer), 22050);
}
