#ifndef BYTEROUTE_BYTEORDER_H
#define BYTEROUTE_BYTEORDER_H

#include <stdint.h>

uint16_t br_read_be16(const uint8_t *buffer);
uint32_t br_read_be32(const uint8_t *buffer);

void br_write_be16(uint8_t *buffer, uint16_t value);
void br_write_be32(uint8_t *buffer, uint32_t value);

#endif