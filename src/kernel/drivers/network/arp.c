#include "arp.h"
#include "arp_cache.h"
#include "rtl8139.h"
#include <stdint.h>
#include "../../memory.h"

#define htons(x) ((uint16_t)((x << 8) | (x >> 8)))
#define htonl(x)  __builtin_bswap32(x)
#define ntohs(x) htons(x)
#define ntohl(x) htonl(x)

#define ARP_HW_ETH   1
#define ARP_PROTO_IP 0x0800
#define ARP_REQUEST  1
#define ARP_REPLY    2

static const uint8_t BROADCAST[6] =
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

typedef struct {
    uint8_t dst[6];
    uint8_t src[6];
    uint16_t type;

    uint16_t hw_type;
    uint16_t proto;
    uint8_t hw_len;
    uint8_t proto_len;
    uint16_t opcode;

    uint8_t sender_mac[6];
    uint32_t sender_ip;
    uint8_t target_mac[6];
    uint32_t target_ip;
} __attribute__((packed)) arp_packet_t;

void arp_init(void)
{
    arp_cache_init();
}

void arp_request(uint32_t ip)
{
    uint8_t packet[42];
    arp_packet_t* arp = (arp_packet_t*)packet;

    // Ethernet header
    memcpy(arp->dst, BROADCAST, 6);
    rtl8139_get_mac(arp->src);

    arp->type = htons(0x0806);

    // ARP header
    arp->hw_type   = htons(1);
    arp->proto     = htons(0x0800);
    arp->hw_len    = 6;
    arp->proto_len = 4;
    arp->opcode    = htons(ARP_REQUEST);

    rtl8139_get_mac(arp->sender_mac);
    arp->sender_ip = htonl(0xC0A80164); // TODO: replace with real IP

    memset(arp->target_mac, 0, 6);
    arp->target_ip = ip;

    rtl8139_send(packet, 42);
}

void arp_handle_packet(uint8_t* packet)
{
    arp_packet_t* arp = (arp_packet_t*)packet;

    if (ntohs(arp->opcode) != ARP_REPLY)
        return;

    arp_cache_add(arp->sender_ip, arp->sender_mac);
}
