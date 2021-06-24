#include <stddef.h>
#include <malloc.h>
#include <string.h>
#include "control.h"


int sens_add_remote(sensor_t *sensor_arr, sensor_t *sensor) {
    sensor_t *iter = sensor_arr;
    while (iter->_next != NULL) {
        if (IN6_ARE_ADDR_EQUAL(&iter->_addr.sin6_addr,&sensor->_addr.sin6_addr)) {
            // already in array
            return 1;
        }
        iter = iter->_next;
    }

    // create new sensor
    iter->_next = sensor;
    return 0;
}

int sens_remove_remote(sensor_t *sensor_arr, sensor_t *sensor) {
    sensor_t *iter = sensor_arr;
    sensor_t *prv  = sensor_arr;

    while (iter->_next != NULL) {
        if (IN6_ARE_ADDR_EQUAL(&iter->_addr.sin6_addr,&sensor->_addr.sin6_addr)) {
            // found element to remove
            prv->_next = iter->_next;
            free(iter);
            return 0;
        }
        prv = iter;
        iter = iter->_next;
    }
    // element not found
    return -1;
}


request_t *prepare_request(in_req_t req_type, const token_t *token, char *payload) {
    request_t *req = malloc(sizeof (request_t));

    req->req_type = req_type;
    req->token = *token;
    req->payload = strdup(payload);

    return req;
}

response_t *prepare_response(in_res_t res_type, const token_t *token, char *payload) {
    response_t *res = malloc(sizeof (response_t));

    res->res_type = res_type;
    res->token = *token;
    res->payload = strdup(payload);

    return res;
}