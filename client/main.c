#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "client.h"

#define PORT 5050
#define SRV_ADDR "2001:db8::1"

int main() {
    struct sockaddr_in6 servaddr;
    setup_client(PORT, SRV_ADDR, &servaddr);

    while(true) {
        snd_rcv(&servaddr);
        sleep(10);
    }
}
