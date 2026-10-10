#ifndef BYTEROUTE_CHECKSUM_H
#define BYTEROUTE_CHECKSUM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t sum;
    uint8_t pending_byte;
    bool has_pending_byte;
} br_checksum_context;

/*
 * Initializes an Internet checksum context.
 *
 * context must not be NULL.
 */
void br_checksum_init(br_checksum_context *context);

/*
 * Adds a block of bytes to an Internet checksum calculation.
 *
 * data may be NULL only when length is zero.
 */
void br_checksum_update(
    br_checksum_context *context,
    const uint8_t *data,
    size_t length
);

/*
 * Finalizes the checksum without modifying the context.
 *
 * The returned value is a host integer representing the 16-bit
 * Internet checksum. Use br_write_be16() when serializing it into
 * a network packet.
 */
uint16_t br_checksum_finalize(const br_checksum_context *context);

/*
 * Calculates an Internet checksum over one contiguous block.
 *
 * data may be NULL only when length is zero.
 */
uint16_t br_checksum_internet(const uint8_t *data, size_t length);

#endif