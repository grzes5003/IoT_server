#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "server.h"
#include "tpool.h"

static const int srv_port = 5500;
static const size_t num_threads = 4;

int main() {
    tpool_t *tm;
    tm = tpool_create(num_threads);

    int srv_sock = setup_server(srv_port);

    while (run_flag) {
        int cli_sock = accept_new_conn(srv_sock);

        check(tpool_add_work(tm, (thread_func_t) handle_connection, (void *) (size_t) cli_sock),
              "Cannot add work to threadpool");
    }

    tpool_wait(tm);
    tpool_destroy(tm);
    return 0;
}
