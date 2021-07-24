#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "wsock32.lib")
#else
#include <arpa/inet.h>
#endif
#include <stdlib.h>
#include "client.h"
#include "log.h"

#define PORT 50500
#if _WIN32
//#define SRV_ADDR "192.168.0.164"
//#define SRV_ADDR "127.0.0.0"
#define SRV_ADDR "fe80::b5ab:3267:5bb7:359"
#else
// #define SRV_ADDR "fc00:1:1::ae"
#define SRV_ADDR "192.168.56.33"
#endif

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
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(PORT);
    if (inet_pton(AF_INET6, SRV_ADDR, &servaddr.sin6_addr) <= 0) {
        return 1;
    }
#else
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
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
