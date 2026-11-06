/* message.h */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

#define HEADER_LENGTH 5 /*bytes*/
#define MAX_PAYLOAD_LENGTH 1048576 /*2^20 bytes*/

typedef enum {
    MSG_LOGIN = 1,
    MSG_CHAT = 2,
    MSG_LOGOUT = 3,
    MSG_ACK = 4,
    MSG_ERROR = 5
} MessageType;

typedef struct Message {
    uint8_t type;
    uint32_t length;
    uint8_t *payload;
} Message;

Message *msg_create(MessageType type, uint32_t length, uint8_t *payload);
void msg_destroy(Message *msg);
uint8_t *msg_encode(const Message *msg, uint32_t *out_length);
#endif /* MESSAGE_H */
