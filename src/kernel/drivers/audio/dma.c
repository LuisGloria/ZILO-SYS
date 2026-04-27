#include "dma.h"
#include "../../arch/i686/io.h"

#define DMA_CHAN1_MASK   0x0A
#define DMA_FLIPFLOP     0x0C
#define DMA_MODE         0x0B
#define DMA_ADDR         0x02
#define DMA_COUNT        0x03
#define DMA_PAGE         0x83

void dma_init(){}

// WARNING: assumes phys_addr is LOW MEMORY
void dma_set_channel_1(void* phys_addr, uint16_t len)
{
    uint32_t addr = (uint32_t)phys_addr;

    uint8_t page     = (addr >> 16) & 0xFF;
    uint16_t offset  = addr & 0xFFFF;
    uint16_t count   = len - 1;

    // mask channel 1
    i686_outb(DMA_CHAN1_MASK, 0x05);

    // reset flip-flop
    i686_outb(DMA_FLIPFLOP, 0);

    // single transfer, channel 1
    i686_outb(DMA_MODE, 0x49);

    // page
    i686_outb(DMA_PAGE, page);

    // address
    i686_outb(DMA_ADDR, offset & 0xFF);
    i686_outb(DMA_ADDR, offset >> 8);

    // count
    i686_outb(DMA_COUNT, count & 0xFF);
    i686_outb(DMA_COUNT, count >> 8);

    // unmask
    i686_outb(DMA_CHAN1_MASK, 0x01);
}
