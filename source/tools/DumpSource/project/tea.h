#pragma once

#include <stdint.h>

int32_t tea_encrypt(uint32_t * dest, const uint32_t * src, const uint32_t * key, int32_t size);
int32_t tea_decrypt(uint32_t * dest, const uint32_t * src, const uint32_t * key, int32_t size);
