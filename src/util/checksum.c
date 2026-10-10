#include "byteroute/checksum.h"

static void br_checksum_add_word(
    br_checksum_context *context,
    uint16_t word
)
{
    context->sum += (uint32_t)word;

    context->sum =
        (context->sum & UINT32_C(0xFFFF)) +
        (context->sum >> 16U);
}

void br_checksum_init(br_checksum_context *context)
{
    context->sum = 0U;
    context->pending_byte = 0U;
    context->has_pending_byte = false;
}

void br_checksum_update(
    br_checksum_context *context,
    const uint8_t *data,
    size_t length
)
{
    if (context->has_pending_byte && length > 0U) {
        const uint16_t word =
            (uint16_t)(
                ((uint16_t)context->pending_byte << 8U) |
                (uint16_t)data[0]
            );

        br_checksum_add_word(context, word);

        context->has_pending_byte = false;

        data += 1;
        length -= 1U;
    }

    while (length >= 2U) {
        const uint16_t word =
            (uint16_t)(
                ((uint16_t)data[0] << 8U) |
                (uint16_t)data[1]
            );

        br_checksum_add_word(context, word);

        data += 2;
        length -= 2U;
    }

    if (length == 1U) {
        context->pending_byte = data[0];
        context->has_pending_byte = true;
    }
}

uint16_t br_checksum_finalize(const br_checksum_context *context)
{
    uint32_t sum = context->sum;

    if (context->has_pending_byte) {
        sum += (uint32_t)context->pending_byte << 8U;
    }

    while ((sum >> 16U) != 0U) {
        sum =
            (sum & UINT32_C(0xFFFF)) +
            (sum >> 16U);
    }

    return (uint16_t)(~sum & UINT32_C(0xFFFF));
}

uint16_t br_checksum_internet(
    const uint8_t *data,
    size_t length
)
{
    br_checksum_context context;

    br_checksum_init(&context);
    br_checksum_update(&context, data, length);

    return br_checksum_finalize(&context);
}