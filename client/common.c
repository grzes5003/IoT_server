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

int check(int exp, const char *msg) {
    if (exp < 0) {
        fprintf(stderr, "%s: %s\n", msg, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return exp;
}