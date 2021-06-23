#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "control.h"
#include <stdbool.h>
#define _REUSEADDR

int check(int exp, const char *msg);

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
 * @return status
 */
int handle_connection(int connfd);
int run_flag;

int sig_pipe();

#endif //SERVER_SERVER_H