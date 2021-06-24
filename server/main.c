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
    int sockfd;
    struct sockaddr_in6	servaddr, cliaddr;

    if ( (sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0){
        fprintf(stderr,"socket error : %s\n", strerror(errno));
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_addr   = in6addr_any;
    servaddr.sin6_port   = htons(SERV_PORT);	/* daytime server */

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        fprintf(stderr,"bind error : %s\n", strerror(errno));
        return 1;
    }


    return 0;
}

