#include "byteroute/checksum.h"

#include <stdint.h>
#include <stdio.h>

static int fail(const char *message)
{
    fprintf(stderr, "%s\n", message);
    return 1;
}

static int test_empty(void)
{
    if (br_checksum_internet(NULL, 0U) != UINT16_C(0xFFFF)) {
        return fail("empty checksum failed");
    }

    return 0;
}

static int test_known_even_vector(void)
{
    const uint8_t data[] = {
        0x00U, 0x01U,
        0xF2U, 0x03U,
        0xF4U, 0xF5U,
        0xF6U, 0xF7U
    };

    if (br_checksum_internet(data, sizeof(data)) !=
        UINT16_C(0x220D)) {
        return fail("known even-length checksum vector failed");
    }

    return 0;
}

static int test_odd_length(void)
{
    const uint8_t data[] = {
        0x01U,
        0x02U,
        0x03U
    };

    if (br_checksum_internet(data, sizeof(data)) !=
        UINT16_C(0xFBFD)) {
        return fail("odd-length checksum failed");
    }

    return 0;
}

static int test_end_around_carry(void)
{
    const uint8_t data[] = {
        0xFFU, 0xFFU,
        0x00U, 0x01U
    };

    if (br_checksum_internet(data, sizeof(data)) !=
        UINT16_C(0xFFFE)) {
        return fail("end-around carry handling failed");
    }

    return 0;
}

static int test_ipv4_header(void)
{
    const uint8_t header[] = {
        0x45U, 0x00U, 0x00U, 0x73U,
        0x00U, 0x00U, 0x40U, 0x00U,
        0x40U, 0x11U, 0x00U, 0x00U,
        0xC0U, 0xA8U, 0x00U, 0x01U,
        0xC0U, 0xA8U, 0x00U, 0xC7U
    };

    if (br_checksum_internet(header, sizeof(header)) !=
        UINT16_C(0xB861)) {
        return fail("IPv4 header checksum calculation failed");
    }

    return 0;
}

static int test_checksum_validation(void)
{
    const uint8_t header[] = {
        0x45U, 0x00U, 0x00U, 0x73U,
        0x00U, 0x00U, 0x40U, 0x00U,
        0x40U, 0x11U, 0xB8U, 0x61U,
        0xC0U, 0xA8U, 0x00U, 0x01U,
        0xC0U, 0xA8U, 0x00U, 0xC7U
    };

    if (br_checksum_internet(header, sizeof(header)) !=
        UINT16_C(0x0000)) {
        return fail("checksum validation failed");
    }

    return 0;
}

static int test_chunked_update(void)
{
    const uint8_t data[] = {
        0x12U,
        0x34U,
        0x56U,
        0x78U,
        0x9AU
    };

    br_checksum_context context;

    br_checksum_init(&context);

    br_checksum_update(&context, &data[0], 1U);
    br_checksum_update(&context, &data[1], 2U);
    br_checksum_update(&context, &data[3], 2U);

    if (br_checksum_finalize(&context) !=
        br_checksum_internet(data, sizeof(data))) {
        return fail("chunked checksum update failed");
    }

    return 0;
}

int main(void)
{
    int failures = 0;

    failures += test_empty();
    failures += test_known_even_vector();
    failures += test_odd_length();
    failures += test_end_around_carry();
    failures += test_ipv4_header();
    failures += test_checksum_validation();
    failures += test_chunked_update();

    if (failures != 0) {
        fprintf(
            stderr,
            "%d checksum test(s) failed\n",
            failures
        );

        return 1;
    }

    return 0;
}