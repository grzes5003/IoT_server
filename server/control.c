#include <stddef.h>
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