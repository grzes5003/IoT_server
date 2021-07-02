#ifndef SERVER_CONTROL_H
#define SERVER_CONTROL_H
#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netinet/in.h>
#endif
#include "common.h"

typedef struct sensor_t {
    struct sockaddr_in6 _addr;
    struct message_t _msg_arr[10];
    struct sensor_t *_next;
} sensor_t;

#define SENSOR_ARR_SIZE 100

/**
 * Finds sensor in linked list of subscribed sensors
 * @param sensor_arr linked list of subscribed sensors
 * @param address of sensor
 * @return pointer to sensor or NULL if non is found
 */
sensor_t *sens_find(sensor_t *sensor_arr, struct in6_addr *address);

/**
 * Add sensor to linked list of subscribed sensors
 * @param sensor_arr linked list of subscribed sensors
 * @param sensor to be added
 * @return status
 */
int sens_add_remote(sensor_t *sensor_arr, sensor_t *sensor);

/**
 * Remove sensor from linked list of subscribed sensors
 * @param sensor_arr linked list of subscribed sensors
 * @param sensor to be removed
 * @return
 */
int sens_remove_remote(sensor_t *sensor_arr, sensor_t *sensor);

int change_mode();



#endif //SERVER_CONTROL_H
