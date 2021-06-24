#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "control.h"
#include <stdbool.h>

#define _REUSEADDR
#define RCV_BUFFSIZE  10000

/**
 * Sets up socket
 * @param port bound to socket
 * @return
 */
int setup_server(int port);
/**
 * Accepts new connection and returns
 * new connected socket
 * @param sockfd socket descriptor
 * @return new connected socket
 */
int accept_new_conn(int sockfd);
/**
 * Handles connection
 * @param connfd socket descriptor
 * @param sensor_arr linked list of sensors
 * @return status
 */
int handle_connection(int connfd, sensor_t *sensor_arr);

int run_flag;

int sig_pipe();

#endif //SERVER_SERVER_H
