#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stddef.h>  // ”√”⁄size_t

void crc32_init(void);
uint32_t crc32_calc(const void* data, size_t len, uint32_t crc);
uint32_t crc32_hash(long num);

#endif