#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include "server.h"
#include "log.h"

int run_flag = true;

int setup_server(int port) {
    int sockfd;
    struct sockaddr_in6 servaddr;

    check((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)), "socket error");
    log_info("created socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_addr = in6addr_any;
    servaddr.sin6_port = htons(port);

    check(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)), "bind error");
    log_info("bound socket");

    return sockfd;
}

int accept_new_conn(int sockfd) {
    return 0;
}

int handle_connection(int connfd, sensor_t *sensor_arr) {
    int n;
    struct sockaddr_in6 cliaddr;
    bzero(&cliaddr, sizeof (struct sockaddr_in6));
    socklen_t srcaddrlen = sizeof cliaddr;
    char *rcvbuff = malloc(RCV_BUFFSIZE);
    char ipv6_human[INET6_ADDRSTRLEN + 1];
    time_t ticks;


    check((n = recvfrom(connfd, (void *) rcvbuff, RCV_BUFFSIZE, 0, (struct sockaddr *) &cliaddr, &srcaddrlen)),
          "recvfrom error");

    inet_ntop(AF_INET6, (struct sockaddr *) &cliaddr.sin6_addr, ipv6_human, sizeof(ipv6_human));
    log_info("Received message from %s", ipv6_human);

    if (n != sizeof (message_t)) {
        log_error("Received corrupted message");
        return 1;
    }
    message_t *recv_msg = (message_t *) rcvbuff;
    log_info("Message: TYPE %i, PAYLOAD %s", recv_msg->msg_type, recv_msg->payload);

    sensor_t *sensor;
    if ((sensor = sens_find(sensor_arr, &cliaddr.sin6_addr)) == NULL) {
        // add sensor to list
        sensor_t _sensor = {cliaddr, {}, NULL};
        sens_add_remote(sensor_arr, &_sensor);
    }

    message_t resp_msg;
    if (recv_msg->msg_type == MSG_HELLO) {
        token_t token = (int64_t) 120;
        resp_msg = *prepare_message(RES_ACK, &token, "ACK1234");
        check(sendto(connfd, &resp_msg, sizeof (message_t), 0, (struct sockaddr *) &cliaddr, srcaddrlen),
              "sendto error");
    }

    return 0;
}

int sig_pipe() {
    run_flag = false;
    return 0;
}
