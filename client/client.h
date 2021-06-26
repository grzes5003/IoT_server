#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <netinet/in.h>

#define MAXLINE 1024
#define SA      struct sockaddr

int setup_client(int port, char *addr, const SA *servaddr, socklen_t servlen);

int snd_rcv(const struct sockaddr_in6 *servaddr);

#endif //CLIENT_CLIENT_H
