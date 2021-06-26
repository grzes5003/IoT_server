#include <stddef.h>
#include <malloc.h>
#include <memory.h>
#include "control.h"

sensor_t *prepare_sensor_arr() {
    static const uint8_t my_addr1[16] = { 0x20, 0x01, 0x08, 0x88, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88 };
    static const uint8_t my_addr2[16] = { 0x20, 0x01, 0x08, 0x88, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };

    sensor_t *head = NULL;
    sensor_t *second = NULL;

    head = (sensor_t*) malloc(sizeof (sensor_t));
    second = (sensor_t*) malloc(sizeof (sensor_t));

    head->_addr.sin6_family = AF_INET6;
    memcpy(head->_addr.sin6_addr.s6_addr, my_addr1, sizeof my_addr1);
    head->_addr.sin6_port = htons(15);
    head->_next = second;

    second->_addr.sin6_family = AF_INET6;
    memcpy(second->_addr.sin6_addr.s6_addr, my_addr2, sizeof my_addr2);
    second->_addr.sin6_port = htons(16);
    second->_next= NULL;

    return head;
}

void test_add_sens_to_remote(sensor_t *arr) {
    static const uint8_t my_addr3[16] = { 0x20, 0x01, 0x08, 0x88, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };
    sensor_t *third = NULL;
    third = (sensor_t*) malloc(sizeof (sensor_t));

    third->_addr.sin6_family = AF_INET6;
    memcpy(third->_addr.sin6_addr.s6_addr, my_addr3, sizeof my_addr3);
    third->_addr.sin6_port = htons(17);
    third->_next= NULL;

    sens_add_remote(arr, third);
    printf("stuff");
}

void test_remove_sens_from_remote(sensor_t *arr) {
    static const uint8_t my_addr2[16] = { 0x20, 0x01, 0x08, 0x88, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };

    sensor_t *second = NULL;
    second = (sensor_t*) malloc(sizeof (sensor_t));

    second->_addr.sin6_family = AF_INET6;
    memcpy(second->_addr.sin6_addr.s6_addr, my_addr2, sizeof my_addr2);
    second->_addr.sin6_port = htons(16);
    second->_next= NULL;

    sens_remove_remote(arr, second);
    printf("stuff 2");
}

void test_deserialize() {
    char data[] = "123456789abcdefgh";
    deserialize_data(data);
}

int main() {
    test_deserialize();
    sensor_t *sensor_arr = prepare_sensor_arr();
    test_add_sens_to_remote(sensor_arr);
    test_remove_sens_from_remote(sensor_arr);
    return 0;
}