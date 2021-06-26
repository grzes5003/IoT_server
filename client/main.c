#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "client.h"

#define PORT 5000
#define SRV_ADDR "fc00:1:1::ae"

int main() {
    struct sockaddr_in6 servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(PORT);
    if (inet_pton(AF_INET6, SRV_ADDR, &servaddr.sin6_addr) <= 0) {
        return 1;
    }

    setup_client(PORT, SRV_ADDR, (SA *) &servaddr, sizeof (servaddr));

//    while(true) {
//        snd_rcv(&servaddr);
//        sleep(10);
//    }
}
