#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "wsock32.lib")
#else
#include <arpa/inet.h>
#endif
#include "server.h"
#include "log.h"


int __cdecl setup_server(int port) {
    struct sockaddr_in hints;
    memset(&hints, 0, sizeof(hints));
#if _IPV6
    struct sockaddr_in6 servaddr;
    hints.sin_family = AF_INET6;

    memset(&servaddr,0, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_addr = in6addr_any;
    servaddr.sin6_port = htons(port);
#else
    struct sockaddr_in servaddr;
    hints.sin_family = AF_INET;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
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
    log_info("created socket");
#else
    int sockfd;

    check((sockfd = socket(hints.sin_family, SOCK_DGRAM, 0)), "socket error");
    log_info("created socket");
#endif
    check(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)), "bind error");
    log_info("bound socket");

    return sockfd;
}

int __cdecl accept_new_conn(int sockfd) {
    return 0;
}

int __cdecl handle_connection(int connfd, sensor_t *sensor_arr) {
    int n;
    struct sockaddr_in6 cliaddr;
    memset(&cliaddr,0 , sizeof (struct sockaddr_in6));
    socklen_t srcaddrlen = sizeof cliaddr;
    char *rcvbuff = malloc(RCV_BUFFSIZE);
    time_t ticks;


    check((n = recvfrom(connfd, (void *) rcvbuff, RCV_BUFFSIZE, 0, (struct sockaddr *) &cliaddr, &srcaddrlen)),
          "recvfrom error");

#if _IPV6
    char ip_human[INET6_ADDRSTRLEN + 1];
    inet_ntop(AF_INET6, (struct sockaddr *) &cliaddr.sin6_addr, ip_human, sizeof(ip_human));
#else
    char ip_human[INET_ADDRSTRLEN + 1];
    inet_ntop(AF_INET, (struct sockaddr *) &cliaddr.sin6_addr, ip_human, sizeof(ip_human));
#endif
    log_info("Received message from %s", ip_human);

    if (n != sizeof (message_t)) {
        log_error("Received corrupted message");
        return 1;
    }
    message_t *recv_msg = (message_t *) rcvbuff;
    log_info("Message: TYPE %i, PAYLOAD %s", recv_msg->msg_type, recv_msg->payload);

    sensor_t *sensor;
    if (NULL == (sensor = sens_find(sensor_arr, &cliaddr.sin6_addr))) {
        /* add sensor to list */
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
