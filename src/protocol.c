#include "protocol.h"
#include <stddef.h>
#include <string.h>

uint16_t checksumCalculator(char *buffer, uint16_t lengthOfBytes) {
    uint8_t sum1 = 0;
    uint8_t sum2 = 0;

    uint16_t i = 0;
    while(i < lengthOfBytes) {
        sum1 = (sum1 + buffer[i]) % 255;
        sum2 = (sum2 + sum1) % 255;

        i++;
    }

    uint16_t checksum = (sum2 << 8) | sum1;
    return checksum;

}

// builds the message in a buffer and returns the size of the message
uint16_t buildMessage(uint8_t messageType, uint16_t lengthOfBytes, char *payload, char *buffer) {
    memcpy(buffer, &messageType, sizeof(uint8_t));

    uint16_t checksum = 0x0000;

    memcpy(buffer + 1, &checksum, sizeof(uint16_t));
    memcpy(buffer + 3, &lengthOfBytes, sizeof(uint16_t));

    if (payload != NULL && lengthOfBytes > 0) {
        memcpy(buffer + 5, payload, lengthOfBytes);
    }

    checksum =  checksumCalculator(buffer, HEADER_SIZE + lengthOfBytes);
    memcpy(buffer + 1, &checksum, sizeof(uint16_t));

    return HEADER_SIZE + lengthOfBytes;
}

// parse message
struct ParsedMessage parseMessage(char *buffer) {
    uint8_t messageType;
    memcpy(&messageType, buffer, sizeof(uint8_t));

    uint16_t checksum;
    memcpy(&checksum, buffer + 1, sizeof(uint16_t));
    
    uint16_t lengthOfBytes;
    memcpy(&lengthOfBytes, buffer + 3, sizeof(uint16_t));

    char *payload = buffer + 5;
    
    // temporary check sum to verify checksum
    uint16_t tempChecksum = 0x0000;
    memcpy(buffer + 1, &tempChecksum, sizeof(uint16_t));
    tempChecksum = checksumCalculator(buffer, HEADER_SIZE + lengthOfBytes);

    memcpy(buffer + 1, &checksum, sizeof(uint16_t));

    bool checksumEqual = (tempChecksum == checksum);

    struct ParsedMessage result;
    result.messageType = messageType;
    result.lengthOfMessage = lengthOfBytes;
    result.payload = payload;
    result.checksumEqual = checksumEqual;

    return result;
}