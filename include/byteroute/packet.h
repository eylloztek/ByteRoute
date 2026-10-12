#ifndef BYTEROUTE_PACKET_H
#define BYTEROUTE_PACKET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BR_PACKET_CAPACITY 2048U
#define BR_PACKET_DEFAULT_HEADROOM 128U

typedef struct {
    uint8_t storage[BR_PACKET_CAPACITY];
    size_t start;
    size_t length;
} br_packet;

void br_packet_init(br_packet *packet);
void br_packet_reset(br_packet *packet);

uint8_t *br_packet_data(br_packet *packet);
const uint8_t *br_packet_const_data(const br_packet *packet);
uint8_t *br_packet_tail(br_packet *packet);

size_t br_packet_length(const br_packet *packet);
size_t br_packet_headroom(const br_packet *packet);
size_t br_packet_tailroom(const br_packet *packet);

bool br_packet_commit(br_packet *packet, size_t length);
bool br_packet_append(br_packet *packet, const void *data, size_t length);
bool br_packet_prepend(br_packet *packet, const void *data, size_t length);
bool br_packet_consume(br_packet *packet, size_t length);

#endif