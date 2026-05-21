#ifndef PROTOCOL_H
#define PROTOCOL_H

// including fixed data type sizes
#include <stdint.h>

// defining constants
#define CONNECT 0x01
#define MSG 0x02
#define DISCONNECT 0x03
#define ACK 0x04
#define ERROR 0x05

struct MessageHeader {
    uint8_t msg_type;
    uint16_t length;
    uint16_t checksum;
};

#define HEADER_SIZE sizeof(struct MessageHeader)

#endif