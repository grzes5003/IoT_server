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

/**
 * Prepares request based on arguments
 * @param req_type Type of request
 * @param token Auth token
 * @param payload Message to be included
 * @return request_t pointer
 */
request_t *prepare_request(in_req_t req_type, const token_t *token, char *payload);

/**
 * Prepares request based on arguments
 * @param res_type Type of request
 * @param token Auth token
 * @param payload Message to be included
 * @return response_t pointer
 */
response_t *prepare_response(in_res_t res_type, const token_t *token, char *payload);

#endif //SERVER_CONTROL_H
