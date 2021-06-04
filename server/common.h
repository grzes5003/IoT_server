#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <stdint.h>

typedef uint64_t token_t;

/* Common type to represent request type */
typedef uint8_t in_req_t;

enum {
    REQ_RETRANS = 0,
#define REQ_RETRANS             REQ_RETRANS
    REQ_READ_SENSOR = 1,
#define REQ_READ_SENSOR         REQ_READ_SENSOR
    REQ_SHUTDOWN = 2,
#define REQ_SHUTDOWN            REQ_SHUTDOWN
    REQ_CHANGE_UPDATE_PRD = 3,
#define REQ_CHANGE_UPDATE_PRD   REQ_CHANGE_UPDATE_PRD
    REQ_BATTERY_STATUS = 5,
#define REQ_BATTERY_STATUS      REQ_BATTERY_STATUS
};

typedef struct request_t {
    in_req_t req_type;
    token_t  token;
} request_t;

/* Common type to represent response type */
typedef uint8_t in_res_t;

enum {
    RES_ACK = 10,
#define RES_ACK                 RES_ACK
    RES_OK = 11,
#define RES_OK                  RES_OK
    RES_ERR = 12,
#define RES_ERR                 RES_ERR
    RES_READ_SENSOR = 13,
#define RES_READ_SENSOR         RES_READ_SENSOR
    RES_BATTERY_STATUS = 15,
#define RES_BATTERY_STATUS      RES_BATTERY_STATUS
};

typedef struct response_t {
    in_res_t res_type;
    token_t  token;
} response_t;

#endif //SERVER_COMMON_H
