#include "byteroute/byteorder.h"

uint16_t br_read_be16(const uint8_t *buffer)
{
    return (uint16_t)(
        ((uint16_t)buffer[0] << 8U) |
        (uint16_t)buffer[1]
    );
}

uint32_t br_read_be32(const uint8_t *buffer)
{
    return ((uint32_t)buffer[0] << 24U) |
           ((uint32_t)buffer[1] << 16U) |
           ((uint32_t)buffer[2] << 8U) |
           (uint32_t)buffer[3];
}

void br_write_be16(uint8_t *buffer, uint16_t value)
{
    buffer[0] = (uint8_t)((value >> 8U) & 0xFFU);
    buffer[1] = (uint8_t)(value & 0xFFU);
}

void br_write_be32(uint8_t *buffer, uint32_t value)
{
    buffer[0] = (uint8_t)((value >> 24U) & 0xFFU);
    buffer[1] = (uint8_t)((value >> 16U) & 0xFFU);
    buffer[2] = (uint8_t)((value >> 8U) & 0xFFU);
    buffer[3] = (uint8_t)(value & 0xFFU);
}