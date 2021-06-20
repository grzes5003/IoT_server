#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "server.h"

int run_flag = true;

int check(int exp, const char *msg) {
    if (exp < 0) {
        fprintf(stderr, "%s: %s\n", msg, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return exp;
}

int sig_pipe() {
    run_flag = false;
    return 0;
}
