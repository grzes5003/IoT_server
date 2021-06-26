#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "common.h"

message_t *prepare_message(in_msg_t msg_type, const token_t *token, char *payload) {
    message_t *msg = malloc(sizeof (message_t));

    msg->msg_type = msg_type;
    msg->token = *token;
    msg->payload = strdup(payload);

    return msg;
}

message_t *deserialize_data(char *data) {
    in_msg_t msg_type;
    memcpy(&msg_type, data, sizeof (in_msg_t));
    char c_token[TOKEN_LEN];
    memcpy(c_token, &data[TOKEN_OFFSET], sizeof (token_t));
    char c_payload[PAYLOAD_LEN];
    memcpy(&c_payload, &data[PAYLOAD_OFFSET], PAYLOAD_LEN);

    return &(message_t){(in_msg_t) data[0], (token_t) c_token, c_payload};
}

int check(int exp, const char *msg) {
    if (exp < 0) {
        fprintf(stderr, "%s: %s\n", msg, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return exp;
}