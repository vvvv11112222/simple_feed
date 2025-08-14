#include <stdio.h>
#include <stdint.h>
#include "hash.h"
static uint32_t crc32_table[256];

// 初始化CRC32表
void crc32_init(void) {
    uint32_t crc;
    for (int i = 0; i < 256; i++) {
        crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0x04C11DB7;  // 多项式
            }
            else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}

uint32_t crc32_calc(const void* data, size_t len, uint32_t crc) {
    const uint8_t* buf = (const uint8_t*)data;
    for (size_t i = 0; i < len; i++) {
        crc = (crc >> 8) ^ crc32_table[(crc ^ buf[i]) & 0xFF];
    }
    return crc;
}

uint32_t crc32_hash(long num) {
    uint32_t crc = 0xFFFFFFFF;
    crc = crc32_calc(&num, sizeof(long), crc);
    return crc ^ 0xFFFFFFFF;
}
