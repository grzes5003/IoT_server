#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <stdint.h>

typedef uint64_t token_t;

typedef uint8_t in_msg_t;

enum {
    MSG_HELLO = 30,
#define MSG_HELLO               MSG_HELLO
};

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
};

typedef struct request_t {
    in_req_t req_type;
    token_t  token;
    char *payload;
} request_t;

/* Common type to represent response type */
typedef uint8_t in_res_t;

enum {
    RES_ACK = 10,
#define RES_ACK                 RES_ACK
    RES_ERR = 12,
#define RES_ERR                 RES_ERR
};

typedef struct response_t {
    in_res_t res_type;
    token_t  token;
    char *payload;
} response_t;

#endif //SERVER_COMMON_H
