#include "../include/message.h"

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/*
 *
 *   Byte:      0       1       2       3       4       5 ...
 *          +-------+-------+-------+-------+-------+-------+
 *          | TYPE  |       (PAYLOAD) LENGTH (4 byte) |     PAYLOAD   |
 *          +-------+-------+-------+-------+-------+-------+
 *
 * 
 */


/* This function creates the message and takes ownership of payload.
 * The caller must not free or use payload after this call.
 */
Message *msg_create(MessageType type, uint32_t length, uint8_t *payload) {
    Message *msg = calloc(1, sizeof(Message));
    if(msg == NULL) return NULL;
    msg->type = type;
    msg->length = length;
    msg->payload = payload;
    return msg;
}

void msg_destroy(Message *msg) {
    if(msg == NULL) return;

    free(msg->payload);
    free(msg);
}

uint8_t *msg_encode(const Message *msg, uint32_t *out_length) {
    if(msg == NULL || out_length == NULL) return NULL;
    if(msg->length > MAX_PAYLOAD_LENGTH) return NULL; // We have to handle errors later
    if(msg->length == 0 && msg->payload == NULL) return NULL;
   
    size_t msg_length = HEADER_LENGTH + msg->length;
    uint8_t *serialized_msg = calloc(1, msg_length);

    if(serialized_msg == NULL) return NULL;
   
    uint32_t net_length = htonl(msg->length);
    
    serialized_msg[0] = msg->type;
    memcpy(serialized_msg+1, &net_length, sizeof(net_length));
    if(msg->length > 0) {
        memcpy(serialized_msg + HEADER_LENGTH, msg->payload, msg->length);
    }
    *out_length = (uint32_t)msg_length;
    return serialized_msg;
}

Message *msg_decode(uint8_t *serialized_msg, size_t serialized_length) {
    if(serialized_msg == NULL) return NULL;
    if(serialized_length < HEADER_LENGTH) return NULL;

    Message *msg = calloc(1, sizeof(Message));
    if(msg == NULL) return NULL;

    msg->type = serialized_msg[0];

    uint32_t net_length;
    memcpy(&net_length, serialized_msg+1, sizeof(net_length));
    msg->length = ntohl(net_length);
    if(msg->length > MAX_PAYLOAD_LENGTH) {
        msg_destroy(msg);
        return NULL;
    }
    
    size_t expected_length = HEADER_LENGTH + msg->length;
    if(expected_length != serialized_length) {
        msg_destroy(msg);
        return NULL;
    }

    msg->payload = calloc(1, msg->length);
    if(msg->payload == NULL) {
        msg_destroy(msg);
        return NULL;
    }
    memcpy(msg->payload, serialized_msg+HEADER_LENGTH, msg->length);
    
    return msg;
}


















