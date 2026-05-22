#include "protocol.h"

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
uint16_t build_message(uint8_t messageType, uint16_t lengthOfBytes, char *payload, char *buffer) {
    memcpy(buffer, &messageType, sizeof(uint8_t));

    uint16_t checksum = 0x0000;

    memcpy(buffer + 1, &checksum, sizeof(uint16_t));
    memcpy(buffer + 3, &lengthOfBytes, sizeof(uint16_t));
    memcpy(buffer + 5, payload, lengthOfBytes);

    checksum =  checksumCalculator(buffer, HEADER_SIZE + lengthOfBytes);
    memcpy(buffer + 1, &checksum, sizeof(uint16_t));

    return HEADER_SIZE + lengthOfBytes;
}