#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "server.h"
#include "tpool.h"
#include "log.h"

#define SERV_PORT 5500
#define RCV_BUFFSIZE  10000

void srv_rcv_loop(int srvsockfd)
{
    int		n;
    struct sockaddr_in6 cliaddr;
    socklen_t	srcaddrlen;
    char *rcvbuff = malloc(RCV_BUFFSIZE);
    char		ipv6_human[INET6_ADDRSTRLEN+1];
    time_t		ticks;

    fprintf(stderr,"Waiting for clients ... \n");
    for (;;) {
        if( (n = recvfrom(srvsockfd, (void *) rcvbuff, RCV_BUFFSIZE, 0, (struct sockaddr *) &cliaddr, &srcaddrlen)) < 0 ){
            fprintf(stderr,"recvfrom error : %s\n", strerror(errno));
            exit(1);
        }

        inet_ntop(AF_INET6, &cliaddr.sin6_addr, ipv6_human, sizeof(ipv6_human));
        printf("Received datagram from %s\n", ipv6_human);


//        if( sendto(sockfd, mesg, strlen(mesg), 0, pcliaddr, len) < 0 ) {
//            fprintf(stderr,"sendto error : %s\n", strerror(errno));
//            //         continue;
//            exit(1);
//        }
    }
}

int main() {
    sensor_t *sensor_arr;

    int servfd = setup_server(15);

    sensor_arr = (sensor_t*) malloc(sizeof (sensor_t));
    sensor_arr->_addr.sin6_family = AF_INET6;
    sensor_arr->_addr.sin6_addr = in6addr_any;
    sensor_arr->_addr.sin6_port = htons(16);
    sensor_arr->_next= NULL;

    log_info("Waiting for incoming messages ...");
    while (true) {
        handle_connection(servfd, sensor_arr);
    }
    return 0;
}

