#include "byteroute/tap.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    const char *requested_name = NULL;
    br_tap tap;

    if (argc > 2) {
        fprintf(
            stderr,
            "Usage: %s [interface-name]\n",
            argv[0]
        );

        return 2;
    }

    if (argc == 2) {
        requested_name = argv[1];
    }

    if (br_tap_open(&tap, requested_name) < 0) {
        perror("br_tap_open");
        return 1;
    }

    printf(
        "TAP interface '%s' opened successfully.\n",
        tap.name
    );

    puts("Press Enter to close the interface.");

    (void)getchar();

    if (br_tap_close(&tap) < 0) {
        perror("br_tap_close");
        return 1;
    }

    return 0;
}