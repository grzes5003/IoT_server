#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "server.h"
#include "tpool.h"
#include "log.h"

#define SERV_PORT 5000
#define RCV_BUFFSIZE  10000

int main() {
    sensor_t *sensor_arr;

    int servfd = setup_server(SERV_PORT);

    sensor_arr = (sensor_t*) malloc(sizeof (sensor_t));
    sensor_arr->_addr.sin6_family = AF_INET6;
    sensor_arr->_addr.sin6_addr = in6addr_any;
    sensor_arr->_addr.sin6_port = htons(5500);
    sensor_arr->_next= NULL;

    log_info("Waiting for incoming messages ...");
    while (true) {
        handle_connection(servfd, sensor_arr);
    }
    return 0;
}

