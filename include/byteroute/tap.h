#ifndef BYTEROUTE_TAP_H
#define BYTEROUTE_TAP_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define BR_TAP_NAME_CAPACITY 16U

typedef struct {
    int fd;
    char name[BR_TAP_NAME_CAPACITY];
} br_tap;

int br_tap_open(br_tap *tap, const char *requested_name);
int br_tap_close(br_tap *tap);

ssize_t br_tap_read(
    br_tap *tap,
    uint8_t *buffer,
    size_t capacity
);

ssize_t br_tap_write(
    br_tap *tap,
    const uint8_t *buffer,
    size_t length
);

#endif