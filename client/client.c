#include <sys/types.h>
#include "client.h"
#include "common.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int setup_client(int port, char *addr) {
    int sockfd, n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    struct sockaddr_in6 servaddr;

    check(sockfd = socket(AF_INET6, SOCK_DGRAM, 0), "socket error");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin6_family = AF_INET6;
    check(inet_pton(AF_INET6, addr, &servaddr.sin6_addr), "inet_pton error");
    servaddr.sin6_port = htons(port);

    // check(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)), "bind error");

    check(connect(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)),
          "connect error");

    check(write(sockfd, sendline, 0), "write error");

    check(n = read(sockfd, recvline, MAXLINE), "read error");


    return sockfd;
}