#ifndef KEY_STORAGE_H
#define KEY_STORAGE_H

#include <stddef.h>
#include <stdint.h>

int store_private_key(const uint8_t *key, size_t key_len);
int load_private_key(uint8_t *key_out, size_t key_len);

#endif // KEY_STORAGE_H
