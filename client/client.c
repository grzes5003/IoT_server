#include <sys/types.h>
#include "client.h"
#include "common.h"
#include "log.h"
#include "sensor.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

/**
 * Setup server, set servaddr
 * @param port used to connect
 * @param addr of server
 * @param servaddr struct sockaddr_in6
 * @return status
 */
int setup_client(int port, char *addr, const SA *servaddr, socklen_t servlen) {
    int sockfd, n;
    char recvline[MAXLINE + 1];

    check(sockfd = socket(AF_INET6, SOCK_DGRAM, 0), "socket error");

    check(connect(sockfd, (SA *) servaddr, servlen),
          "connect error");

    printf("stuff");
    token_t token = (int64_t) 120;
    message_t hello_message = *prepare_message(MSG_HELLO, &token, "1234567");
    check(send(sockfd, &hello_message, sizeof (message_t), 0), "write error");


//    if (hello_response.res_type == RES_ERR) {
//        log_warn("Got RES_ERR from server");
//        return -1;
//    }
//
//    if (hello_response.res_type != RES_ACK) {
//        log_error("Got unexpected response from server");
//        return -1;
//    }

    return 0;
}

/**
 * Send current read and listen
 * for responses for particular time
 * @param servaddr address of server
 * @return status
 */
int snd_rcv(const struct sockaddr_in6 *servaddr) {
    int sockfd, n;

    check(sockfd = socket(AF_INET6, SOCK_DGRAM, 0), "socket error");
    check(connect(sockfd, (const struct sockaddr *) &servaddr, sizeof(struct sockaddr)),
          "connect error");

    char buffer[9]; bzero(&buffer, sizeof(char) * 9);
    snprintf(buffer, sizeof buffer, "%f", get_temperature());
    message_t *msg_temper = prepare_message(MSG_READ, NULL, buffer);
    check(write(sockfd, msg_temper, sizeof(message_t)), "write error");

    int msec = 0, trigger = 5000;
    clock_t before = clock();

    do {
//        request_t request;
//        bzero(&request, sizeof(response_t));
//        check(read(sockfd, &request.req_type, sizeof(in_res_t)), "read error");
//        check(read(sockfd, &request.token, sizeof(token_t)), "read error");
//        check(read(sockfd, &request.payload, sizeof(uint8_t) + sizeof(uint64_t)), "read error");

        clock_t difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
    } while ( msec < trigger );

    return 0;
}