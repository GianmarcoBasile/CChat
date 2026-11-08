#include "test_message.h"
#include <assert.h>
#include <stdio.h>

void test_msg_create(MessageType type, uint32_t length, uint8_t *payload) {
    Message *msg = msg_create(type, length, payload);
    
    // Check if fields are correct
    assert(msg->type == type);
    assert(msg->length == length);
    assert(msg->payload == payload);

    //Check if total length is correct
    uint32_t out_length;
    uint8_t *serialized_msg = msg_encode(msg, &out_length);
    assert(out_length == HEADER_LENGTH + msg->length);
}

int main(void) {
    test_msg_create(MSG_CHAT, 5, "Ciao");
}