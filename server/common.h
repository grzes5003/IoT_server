#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <stdint.h>

typedef uint64_t token_t;

typedef uint8_t in_msg_t;

enum {
    MSG_HELLO = 30,
#define MSG_HELLO               MSG_HELLO
    MSG_READ = 31,
#define MSG_READ                MSG_READ
};

typedef struct message_t {
    in_msg_t msg_type;
    token_t  token;
    char *payload;
} message_t;

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

/**
 * Prepares message based on arguments
 * @param res_type Type of message
 * @param token Auth token
 * @param payload Message to be included
 * @return message_t pointer
 */
message_t *prepare_message(in_msg_t msg_type, const token_t *token, char *payload);

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

int check(int exp, const char *msg);

#endif //SERVER_COMMON_H
