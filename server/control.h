#ifndef SERVER_CONTROL_H
#define SERVER_CONTROL_H
#include <netinet/in.h>
#include "common.h"

typedef struct sensor_t {
    struct sockaddr_in6 _addr;
    struct request_t _req_arr[10];
    struct sensor_t *_next;
} sensor_t;

#define SENSOR_ARR_SIZE 100

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
