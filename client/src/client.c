#include <sys/types.h>
#include "client.h"
#include "common.h"
#include "log.h"
#include "sensor.h"
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>


message_t *recv_message(int sockfd, int timeout) {
    int n;
    char *rcvbuff = malloc(RCV_BUFFSIZE);
    memset(rcvbuff, 0, RCV_BUFFSIZE);

    struct sockaddr_in6 srvaddr;
    socklen_t srcaddrlen;
    memset(&srvaddr, 0, srcaddrlen);

#ifndef _TIMEOUT_SOCK
    struct sigaction sact;

    sigemptyset(&sact.sa_mask);
    bzero(&sact, sizeof (struct sigaction));
    sact.sa_flags = 0;
    sact.sa_handler = handler_SIGALARM;
    sigaction(SIGALRM, &sact, NULL);
    alarm(timeout);
#endif

#if _WIN32
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
    BOOL bNewBehavior = FALSE;
    DWORD dwBytesReturned = 0;
    WSAIoctl(sockfd, SIO_UDP_CONNRESET,
             &bNewBehavior, sizeof bNewBehavior,
             NULL, 0, &dwBytesReturned,
             NULL, NULL);

    int _srcaddrlen = sizeof(srvaddr);
    if(SOCKET_ERROR == (n = recvfrom(sockfd, (void *) rcvbuff,
                                     RCV_BUFFSIZE, 0,
                                     (struct sockaddr *) &srvaddr, &_srcaddrlen))) {
        if (EAGAIN == errno) {
            return NULL;
        }
        if (WSAETIMEDOUT == WSAGetLastError()) {
            return NULL;
        }
        fprintf(stderr, "recvfrom error: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
#else
    if((n = recvfrom(sockfd, (void *) rcvbuff, RCV_BUFFSIZE, 0, (struct sockaddr *) &srvaddr, &srcaddrlen)) < 0) {
        if (errno == EAGAIN) {
            return NULL;
        }
        fprintf(stderr, "recvfrom error: %s \n", strerror(errno));
        exit(EXIT_FAILURE);
    }
#endif
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
#if _WIN32
    SOCKET sockfd;
#else
    int sockfd;
#endif

    sockfd = get_sockfd();

#if _WIN32
    if(SOCKET_ERROR == connect(sockfd, (SA *) servaddr, servlen)) {
        log_error("connect failed with error: %d", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
#else
    check(connect(sockfd, (SA *) servaddr, servlen),
          "connect error");
#endif
    token_t token = (int64_t) 120;
    message_t hello_message = *prepare_message(MSG_HELLO, &token, "1234567");
#if _WIN32
    if(SOCKET_ERROR == send(sockfd, (const void *) &hello_message, sizeof (message_t), 0)) {
        log_error("send failed with error: %d", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
#else
    check(send(sockfd, &hello_message, sizeof (message_t), 0), "write error");
#endif

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

    char buffer[9]; memset(&buffer, 0, sizeof(char) * 9);
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
    struct sockaddr_in hints;
    memset(&hints, 0, sizeof(hints));
#if _IPV6
    hints.sin_family = AF_INET6;
#else
    hints.sin_family = AF_INET;
#endif

#if _WIN32
    SOCKET sockfd = INVALID_SOCKET;
    WSADATA wsaData;

    check(WSAStartup(MAKEWORD(2,2), &wsaData),
          "WSAStartup failed with error");

    if(INVALID_SOCKET == (sockfd = socket(hints.sin_family, SOCK_DGRAM, 0))) {
        log_error("socket failed with error: %ld", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
#else
    int sockfd;
    check(sockfd = socket(hints.sin_family, SOCK_DGRAM, 0), "socket error");
#endif

#ifdef _TIMEOUT_SOCK
# if _WIN32
    DWORD tv = LISTEN_TIMEOUT * 1000;
# else
    struct timeval tv;
    tv.tv_sec = LISTEN_TIMEOUT;
    tv.tv_usec = 0;
# endif
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
#endif
    return sockfd;
}
