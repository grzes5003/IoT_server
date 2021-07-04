#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "control.h"
#include <stdbool.h>

#define _REUSEADDR
#define SA struct sockaddr
#define RCV_BUFFSIZE  10000
#define _IPV6 1

/**
 * Sets up socket
 * @param port bound to socket
 * @return
 */
int __cdecl setup_server(int port);
/**
 * Accepts new connection and returns
 * new connected socket
 * @param sockfd socket descriptor
 * @return new connected socket
 */
int __cdecl accept_new_conn(int sockfd);
/**
 * Handles connection
 * @param connfd socket descriptor
 * @param sensor_arr linked list of sensors
 * @return status
 */
int __cdecl handle_connection(int connfd, sensor_t *sensor_arr);

#endif //SERVER_SERVER_H
