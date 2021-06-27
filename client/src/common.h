#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <stdint.h>

typedef uint64_t token_t;

/* Common type to represent message type */
typedef uint8_t in_msg_t;

#define TOKEN_LEN 8
#define TOKEN_OFFSET 1
#define PAYLOAD_LEN 9
#define PAYLOAD_OFFSET 9
#define MESSAGE_LEN 18

typedef struct message_t {
    in_msg_t msg_type;
    token_t  token;
    char payload[PAYLOAD_LEN];
} message_t;

enum {
    MSG_HELLO = 30,
#define MSG_HELLO               MSG_HELLO
    MSG_READ = 31,
#define MSG_READ                MSG_READ
};

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

enum {
    RES_ACK = 10,
#define RES_ACK                 RES_ACK
    RES_ERR = 12,
#define RES_ERR                 RES_ERR
};

/**
 * Prepares message based on arguments
 * @param res_type Type of message
 * @param token Auth token
 * @param payload Message to be included
 * @return message_t pointer
 */
message_t *prepare_message(in_msg_t msg_type, const token_t *token, char *payload);

/**
 * @bug NOT WORKING RN
 * Deserializes data into msg type
 * @param rcv_data buffer
 * @return pointer to new message
 */
message_t *deserialize_data(char *data);

int check(int exp, const char *msg);

#endif //SERVER_COMMON_H
