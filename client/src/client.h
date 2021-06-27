#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <netinet/in.h>
#include <stdbool.h>
#include "common.h"

#define MAXLINE 1024
#define SA      struct sockaddr
#define LISTEN_TIMEOUT 5
#define RCV_BUFFSIZE  10000
#define _TIMEOUT_SOCK

/**
 * Setup server, set servaddr
 * @param port used to connect
 * @param addr of server
 * @param servaddr struct sockaddr_in6
 * @return status
 */
int setup_client(int port, char *addr, const SA *servaddr, socklen_t servlen);

/**
 * Send current read and listen
 * for responses for particular time
 * @param servaddr address of server
 * @return status
 */
int snd_rcv(const SA *servaddr, socklen_t servlen, int *update_prd, bool *run_flag);

void handler_SIGALARM(int sig);

message_t *recv_message(int sockfd, int timeout);

int get_sockfd();

#endif //CLIENT_CLIENT_H
