#include <stddef.h>
#include <malloc.h>
#include <memory.h>
#include "control.h"

int main() {
    static const uint8_t my_addr1[16] = { 0x20, 0x01, 0x08, 0x88, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88 };
    static const uint8_t my_addr2[16] = { 0x20, 0x01, 0x08, 0x88, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };
    static const uint8_t my_addr3[16] = { 0x20, 0x01, 0x08, 0x88, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };

    sensor_t *head = NULL;
    sensor_t *second = NULL;
    sensor_t *third = NULL;

    head = (sensor_t*) malloc(sizeof (sensor_t));
    second = (sensor_t*) malloc(sizeof (sensor_t));
    third = (sensor_t*) malloc(sizeof (sensor_t));

    head->_addr.sin6_family = AF_INET6;
    memcpy(head->_addr.sin6_addr.s6_addr, my_addr1, sizeof my_addr1);
    head->_addr.sin6_port = htons(15);
    head->_next = second;

    second->_addr.sin6_family = AF_INET6;
    memcpy(second->_addr.sin6_addr.s6_addr, my_addr2, sizeof my_addr2);
    second->_addr.sin6_port = htons(16);
    second->_next= NULL;

    third->_addr.sin6_family = AF_INET6;
    memcpy(second->_addr.sin6_addr.s6_addr, my_addr3, sizeof my_addr3);
    third->_addr.sin6_port = htons(17);
    third->_next= NULL;

    sens_add_remote(head, third);
    return 0;
}