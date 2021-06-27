#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "client.h"
#include "log.h"

#define PORT 5000
// #define SRV_ADDR "fc00:1:1::ae"
#define SRV_ADDR "192.168.56.33"

int main() {
    int update_prd = 10;
    bool run_flag = true;

    char *srv_addr = getenv("SRV_ADDR");
    if (srv_addr == NULL) {
        log_info("env SRV_ADDR not defined, using %s instead", SRV_ADDR);
        srv_addr = SRV_ADDR;
    } else {
        log_info("server @ %s", srv_addr);
    }
#if _IPV6
    struct sockaddr_in6 servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(PORT);
    if (inet_pton(AF_INET6, SRV_ADDR, &servaddr.sin6_addr) <= 0) {
        return 1;
    }
#else
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, srv_addr, &servaddr.sin_addr) <= 0) {
        return 1;
    }
#endif

    setup_client(PORT, srv_addr, (SA *) &servaddr, sizeof (servaddr));

    while(run_flag) {
        snd_rcv((SA *) &servaddr, sizeof (servaddr), &update_prd, &run_flag);
        sleep(update_prd);
    }
    return 0;
}
