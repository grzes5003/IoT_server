#include <sys/types.h>
#include "client.h"
#include "common.h"
#include "log.h"
#include "sensor.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>


message_t *recv_message(int sockfd, int timeout) {
    int n;
    char *rcvbuff = malloc(RCV_BUFFSIZE);
    bzero(rcvbuff, RCV_BUFFSIZE);

    struct sockaddr_in6 srvaddr;
    socklen_t srcaddrlen;
    struct sigaction sact;

#ifndef _TIMEOUT_SOCK
    sigemptyset(&sact.sa_mask);
    bzero(&sact, sizeof (struct sigaction));
    sact.sa_flags = 0;
    sact.sa_handler = handler_SIGALARM;
    sigaction(SIGALRM, &sact, NULL);
    alarm(timeout);
#endif

    if((n = recvfrom(sockfd, (void *) rcvbuff, RCV_BUFFSIZE, 0, (struct sockaddr *) &srvaddr, &srcaddrlen)) < 0) {
        if (errno == EAGAIN) {
            return NULL;
        }
        fprintf(stderr, "recvfrom error: %s \n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (!n) {
//        log_info("No message was received");
        return NULL;
    }
    if (n != sizeof (message_t)) {
//        log_error("Received corrupted message");
        return NULL;
    }

    return (message_t *) rcvbuff;
}

int setup_client(int port, char *addr, const SA *servaddr, socklen_t servlen) {
    int sockfd, n;
    char recvline[MAXLINE + 1];

    sockfd = get_sockfd();

    check(connect(sockfd, (SA *) servaddr, servlen),
          "connect error");

    token_t token = (int64_t) 120;
    message_t hello_message = *prepare_message(MSG_HELLO, &token, "1234567");
    check(send(sockfd, &hello_message, sizeof (message_t), 0), "write error");


    message_t *msg = recv_message(sockfd, LISTEN_TIMEOUT);

    if (msg == NULL) {
        printf("got null\n");
        return 0;
    } else if (msg->msg_type == RES_ERR) {
        log_warn("Got RES_ERR from server");
        return -1;
    } else if (msg->msg_type != RES_ACK) {
        log_error("Got unexpected response from server");
        return -1;
    } else if (msg->msg_type == RES_ACK) {
        log_info("got RES_ACK from server");
    }

    return 0;
}

int snd_rcv(const SA *servaddr, socklen_t servlen, int *update_prd, bool *run_flag) {
    int sockfd = get_sockfd();

    check(connect(sockfd, (SA *) servaddr, servlen),
          "connect error");

    char buffer[9]; bzero(&buffer, sizeof(char) * 9);
    snprintf(buffer, sizeof buffer, "%f", get_temperature());
    token_t token = (int64_t) 120;
    message_t msg_temper = *prepare_message(MSG_READ, &token, buffer);
    check(send(sockfd, &msg_temper, sizeof(message_t), 0), "write error");
    log_info("sent temperature %s", buffer);

    int trigger = 10;
    clock_t start_time = clock();
    clock_t delta;
    do {
        delta = (clock() - start_time) / CLOCKS_PER_SEC;

        message_t *msg = recv_message(sockfd, LISTEN_TIMEOUT);
        if (msg == NULL) {
            if (delta < trigger) break;
            continue;
        } else if (msg->msg_type == REQ_READ_SENSOR || msg->msg_type == REQ_RETRANS) {
            check(send(sockfd, &msg_temper, sizeof(message_t), 0), "write error");
        } else if (msg->msg_type == REQ_CHANGE_UPDATE_PRD) {
            char *end;
            *update_prd = strtol(msg->payload, &end, 10);
        } else if (msg->msg_type == REQ_SHUTDOWN) {
            *run_flag = false;
            return 0;
        }
    } while ( delta < trigger );

    return 0;
}

void handler_SIGALARM(int sig) {
    printf("   Signal handler_SIGALARM called for signal %d\n", sig);
}

int get_sockfd() {
    int sockfd;

#if _IPV6
    check(sockfd = socket(AF_INET6, SOCK_DGRAM, 0), "socket error");
#else
    check(sockfd = socket(AF_INET6, SOCK_DGRAM, 0), "socket error");
#endif

#ifdef _TIMEOUT_SOCK
    struct timeval tv;
    tv.tv_sec = LISTEN_TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
#endif

    return sockfd;
}