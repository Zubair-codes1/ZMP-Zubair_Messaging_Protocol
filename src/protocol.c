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