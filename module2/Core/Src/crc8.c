#include "crc8.h"

uint8_t crc_8(const uint8_t *buffer, const uint8_t size ){
    int i;
    uint8_t crc = 0x0;
    for (int ind=0; ind<size; ind++){
        crc = crc^buffer[ind];
        for (i=0;i<8;i++){
            if (crc & 0x80)
                crc = (crc << 1) ^ POLY;
            else
                crc = crc << 1;
        }
    }
    return crc;
}
