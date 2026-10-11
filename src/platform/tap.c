#define _GNU_SOURCE

#include "byteroute/tap.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

_Static_assert(
    BR_TAP_NAME_CAPACITY == IFNAMSIZ,
    "BR_TAP_NAME_CAPACITY must match Linux IFNAMSIZ"
);

int br_tap_open(br_tap *tap, const char *requested_name)
{
    struct ifreq ifr = {0};

    if (tap == NULL) {
        errno = EINVAL;
        return -1;
    }

    tap->fd = -1;
    tap->name[0] = '\0';

    if (requested_name != NULL) {
        const size_t name_length = strlen(requested_name);

        if (name_length == 0U) {
            errno = EINVAL;
            return -1;
        }

        if (name_length >= BR_TAP_NAME_CAPACITY) {
            errno = ENAMETOOLONG;
            return -1;
        }

        memcpy(
            ifr.ifr_name,
            requested_name,
            name_length + 1U
        );
    }

    ifr.ifr_flags = (short)(IFF_TAP | IFF_NO_PI);

    const int fd = open(
        "/dev/net/tun",
        O_RDWR | O_CLOEXEC
    );

    if (fd < 0) {
        return -1;
    }

    if (ioctl(fd, TUNSETIFF, &ifr) < 0) {
        const int saved_errno = errno;

        (void)close(fd);

        errno = saved_errno;
        return -1;
    }

    tap->fd = fd;

    memcpy(
        tap->name,
        ifr.ifr_name,
        BR_TAP_NAME_CAPACITY
    );

    tap->name[BR_TAP_NAME_CAPACITY - 1U] = '\0';

    return 0;
}

int br_tap_close(br_tap *tap)
{
    if (tap == NULL || tap->fd < 0) {
        errno = EINVAL;
        return -1;
    }

    const int result = close(tap->fd);

    if (result < 0) {
        const int saved_errno = errno;

        tap->fd = -1;
        tap->name[0] = '\0';

        errno = saved_errno;
        return -1;
    }

    tap->fd = -1;
    tap->name[0] = '\0';

    return 0;
}

ssize_t br_tap_read(
    br_tap *tap,
    uint8_t *buffer,
    size_t capacity
)
{
    if (
        tap == NULL ||
        tap->fd < 0 ||
        (buffer == NULL && capacity > 0U)
    ) {
        errno = EINVAL;
        return -1;
    }

    return read(
        tap->fd,
        buffer,
        capacity
    );
}

ssize_t br_tap_write(
    br_tap *tap,
    const uint8_t *buffer,
    size_t length
)
{
    if (
        tap == NULL ||
        tap->fd < 0 ||
        (buffer == NULL && length > 0U)
    ) {
        errno = EINVAL;
        return -1;
    }

    return write(
        tap->fd,
        buffer,
        length
    );
}