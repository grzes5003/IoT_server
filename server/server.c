#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <string.h>
#include <errno.h>

int check(int exp, const char *msg) {
    if (exp < 0) {
        fprintf(stderr, "%s: %s\n", msg, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return exp;
}

