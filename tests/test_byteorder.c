#include "byteroute/byteorder.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int test_read_be16(void)
{
    const uint8_t zero[] = {0x00U, 0x00U};
    const uint8_t value[] = {0x12U, 0x34U};
    const uint8_t maximum[] = {0xFFU, 0xFFU};

    if (br_read_be16(zero) != UINT16_C(0x0000)) {
        fprintf(stderr, "br_read_be16 failed for zero\n");
        return 1;
    }

    if (br_read_be16(value) != UINT16_C(0x1234)) {
        fprintf(stderr, "br_read_be16 failed for 0x1234\n");
        return 1;
    }

    if (br_read_be16(maximum) != UINT16_C(0xFFFF)) {
        fprintf(stderr, "br_read_be16 failed for maximum value\n");
        return 1;
    }

    return 0;
}

static int test_read_be32(void)
{
    const uint8_t zero[] = {0x00U, 0x00U, 0x00U, 0x00U};
    const uint8_t value[] = {0x12U, 0x34U, 0x56U, 0x78U};
    const uint8_t maximum[] = {0xFFU, 0xFFU, 0xFFU, 0xFFU};

    if (br_read_be32(zero) != UINT32_C(0x00000000)) {
        fprintf(stderr, "br_read_be32 failed for zero\n");
        return 1;
    }

    if (br_read_be32(value) != UINT32_C(0x12345678)) {
        fprintf(stderr, "br_read_be32 failed for 0x12345678\n");
        return 1;
    }

    if (br_read_be32(maximum) != UINT32_C(0xFFFFFFFF)) {
        fprintf(stderr, "br_read_be32 failed for maximum value\n");
        return 1;
    }

    return 0;
}

static int test_write_be16(void)
{
    uint8_t buffer[2] = {0U};
    const uint8_t expected[] = {0x12U, 0x34U};

    br_write_be16(buffer, UINT16_C(0x1234));

    if (memcmp(buffer, expected, sizeof(expected)) != 0) {
        fprintf(stderr, "br_write_be16 failed\n");
        return 1;
    }

    return 0;
}

static int test_write_be32(void)
{
    uint8_t buffer[4] = {0U};
    const uint8_t expected[] = {0x12U, 0x34U, 0x56U, 0x78U};

    br_write_be32(buffer, UINT32_C(0x12345678));

    if (memcmp(buffer, expected, sizeof(expected)) != 0) {
        fprintf(stderr, "br_write_be32 failed\n");
        return 1;
    }

    return 0;
}

static int test_round_trip(void)
{
    uint8_t buffer16[2] = {0U};
    uint8_t buffer32[4] = {0U};

    const uint16_t value16 = UINT16_C(0xABCD);
    const uint32_t value32 = UINT32_C(0x89ABCDEF);

    br_write_be16(buffer16, value16);
    br_write_be32(buffer32, value32);

    if (br_read_be16(buffer16) != value16) {
        fprintf(stderr, "16-bit byte-order round trip failed\n");
        return 1;
    }

    if (br_read_be32(buffer32) != value32) {
        fprintf(stderr, "32-bit byte-order round trip failed\n");
        return 1;
    }

    return 0;
}

int main(void)
{
    int failures = 0;

    failures += test_read_be16();
    failures += test_read_be32();
    failures += test_write_be16();
    failures += test_write_be32();
    failures += test_round_trip();

    if (failures != 0) {
        fprintf(stderr, "%d byte-order test(s) failed\n", failures);
        return 1;
    }

    return 0;
}