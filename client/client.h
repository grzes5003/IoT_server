#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <netinet/in.h>

#define MAXLINE 1024

int setup_client(int port, char *addr, struct sockaddr_in6 *servaddr);

int snd_rcv(const struct sockaddr_in6 *servaddr);

#endif //CLIENT_CLIENT_H
