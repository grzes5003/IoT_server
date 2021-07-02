#include <stddef.h>
#include <malloc.h>
#include "control.h"


sensor_t *sens_find(sensor_t *sensor_arr, struct in6_addr *address) {
    sensor_t *iter = sensor_arr;
    while (iter->_next != NULL) {
        if (IN6_ARE_ADDR_EQUAL(&iter->_addr.sin6_addr,
                               address)) {
            // found sensor
            return iter;
        }
        iter = iter->_next;
    }
    return NULL;
}

int sens_add_remote(sensor_t *sensor_arr, sensor_t *sensor) {
    sensor_t *iter = sensor_arr;
    while (iter->_next != NULL) {
        if (IN6_ARE_ADDR_EQUAL(&iter->_addr.sin6_addr,
                               &sensor->_addr.sin6_addr)) {
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
        if (IN6_ARE_ADDR_EQUAL(&iter->_addr.sin6_addr,
                               &sensor->_addr.sin6_addr)) {
            // found element to remove
            prv->_next = iter->_next;
            free(iter);
            return 0;
        }
        prv = iter;
        iter = iter->_next;
    }
    /* element not found */
    return -1;
}
