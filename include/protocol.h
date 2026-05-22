#ifndef PROTOCOL_H
#define PROTOCOL_H

// including fixed data type sizes
#include <stdint.h>
#include <stdbool.h>

// defining constants
#define CONNECT 0x01
#define MSG 0x02
#define DISCONNECT 0x03
#define ACK 0x04
#define ERROR 0x05

struct MessageHeader {
    uint8_t messageType;
    uint16_t lengthOfMessage;
    uint16_t checksum;
};

struct ParsedMessage {
    uint8_t messageType;
    uint16_t lengthOfMessage;
    char *payload;
    bool checksumEqual;
};

#define HEADER_SIZE sizeof(struct MessageHeader)

uint16_t checksumCalculator(char *buffer, uint16_t lengthOfBytes);

uint16_t buildMessage(uint8_t messageType, uint16_t lengthOfBytes, char *payload, char *buffer);

struct ParsedMessage parseMessage(char *buffer);

#endif