#include "byteroute/packet.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int fail(const char *message)
{
    fprintf(stderr, "%s\n", message);
    return 1;
}

static int check_invariant(const br_packet *packet)
{
    const size_t total =
        br_packet_headroom(packet) +
        br_packet_length(packet) +
        br_packet_tailroom(packet);

    if (total != BR_PACKET_CAPACITY) {
        return fail("packet capacity invariant failed");
    }

    return 0;
}

static int test_initial_state(void)
{
    br_packet packet;

    br_packet_init(&packet);

    if (br_packet_length(&packet) != 0U) {
        return fail("initial packet length is not zero");
    }

    if (br_packet_headroom(&packet) !=
        BR_PACKET_DEFAULT_HEADROOM) {
        return fail("initial packet headroom is incorrect");
    }

    if (br_packet_tailroom(&packet) !=
        BR_PACKET_CAPACITY - BR_PACKET_DEFAULT_HEADROOM) {
        return fail("initial packet tailroom is incorrect");
    }

    return check_invariant(&packet);
}

static int test_append(void)
{
    br_packet packet;
    const uint8_t payload[] = {
        0x11U,
        0x22U,
        0x33U,
        0x44U
    };

    br_packet_init(&packet);

    if (!br_packet_append(
            &packet,
            payload,
            sizeof(payload))) {
        return fail("packet append failed");
    }

    if (br_packet_length(&packet) != sizeof(payload)) {
        return fail("packet length after append is incorrect");
    }

    if (memcmp(
            br_packet_const_data(&packet),
            payload,
            sizeof(payload)) != 0) {
        return fail("packet payload after append is incorrect");
    }

    return check_invariant(&packet);
}

static int test_prepend(void)
{
    br_packet packet;

    const uint8_t payload[] = {
        0xAAU,
        0xBBU,
        0xCCU
    };

    const uint8_t header[] = {
        0x01U,
        0x02U
    };

    const uint8_t expected[] = {
        0x01U,
        0x02U,
        0xAAU,
        0xBBU,
        0xCCU
    };

    br_packet_init(&packet);

    if (!br_packet_append(
            &packet,
            payload,
            sizeof(payload))) {
        return fail("payload append before prepend failed");
    }

    if (!br_packet_prepend(
            &packet,
            header,
            sizeof(header))) {
        return fail("packet prepend failed");
    }

    if (br_packet_length(&packet) != sizeof(expected)) {
        return fail("packet length after prepend is incorrect");
    }

    if (memcmp(
            br_packet_const_data(&packet),
            expected,
            sizeof(expected)) != 0) {
        return fail("packet contents after prepend are incorrect");
    }

    if (br_packet_headroom(&packet) !=
        BR_PACKET_DEFAULT_HEADROOM - sizeof(header)) {
        return fail("packet headroom after prepend is incorrect");
    }

    return check_invariant(&packet);
}

static int test_commit(void)
{
    br_packet packet;

    const uint8_t expected[] = {
        0xDEU,
        0xADU,
        0xBEU,
        0xEFU
    };

    br_packet_init(&packet);

    uint8_t *tail = br_packet_tail(&packet);

    memcpy(
        tail,
        expected,
        sizeof(expected)
    );

    if (!br_packet_commit(
            &packet,
            sizeof(expected))) {
        return fail("packet commit failed");
    }

    if (br_packet_length(&packet) != sizeof(expected)) {
        return fail("packet length after commit is incorrect");
    }

    if (memcmp(
            br_packet_const_data(&packet),
            expected,
            sizeof(expected)) != 0) {
        return fail("committed packet contents are incorrect");
    }

    return check_invariant(&packet);
}

static int test_consume(void)
{
    br_packet packet;

    const uint8_t data[] = {
        0x10U,
        0x20U,
        0x30U,
        0x40U,
        0x50U
    };

    br_packet_init(&packet);

    if (!br_packet_append(
            &packet,
            data,
            sizeof(data))) {
        return fail("append before consume failed");
    }

    if (!br_packet_consume(&packet, 2U)) {
        return fail("packet consume failed");
    }

    if (br_packet_length(&packet) != 3U) {
        return fail("packet length after consume is incorrect");
    }

    if (br_packet_const_data(&packet)[0] != 0x30U) {
        return fail("packet data pointer after consume is incorrect");
    }

    if (br_packet_headroom(&packet) !=
        BR_PACKET_DEFAULT_HEADROOM + 2U) {
        return fail("packet headroom after consume is incorrect");
    }

    return check_invariant(&packet);
}

static int test_bounds_checks(void)
{
    br_packet packet;
    uint8_t byte = 0U;

    br_packet_init(&packet);

    if (br_packet_prepend(
            &packet,
            &byte,
            br_packet_headroom(&packet) + 1U)) {
        return fail("prepend exceeded packet headroom");
    }

    if (br_packet_commit(
            &packet,
            br_packet_tailroom(&packet) + 1U)) {
        return fail("commit exceeded packet tailroom");
    }

    if (br_packet_consume(&packet, 1U)) {
        return fail("consume exceeded packet length");
    }

    return check_invariant(&packet);
}

static int test_reset(void)
{
    br_packet packet;

    const uint8_t data[] = {
        0x01U,
        0x02U,
        0x03U
    };

    br_packet_init(&packet);

    if (!br_packet_append(
            &packet,
            data,
            sizeof(data))) {
        return fail("append before reset failed");
    }

    if (!br_packet_consume(&packet, 1U)) {
        return fail("consume before reset failed");
    }

    br_packet_reset(&packet);

    if (br_packet_length(&packet) != 0U) {
        return fail("packet reset did not clear length");
    }

    if (br_packet_headroom(&packet) !=
        BR_PACKET_DEFAULT_HEADROOM) {
        return fail("packet reset did not restore headroom");
    }

    return check_invariant(&packet);
}

int main(void)
{
    int failures = 0;

    failures += test_initial_state();
    failures += test_append();
    failures += test_prepend();
    failures += test_commit();
    failures += test_consume();
    failures += test_bounds_checks();
    failures += test_reset();

    if (failures != 0) {
        fprintf(
            stderr,
            "%d packet test(s) failed\n",
            failures
        );

        return 1;
    }

    return 0;
}