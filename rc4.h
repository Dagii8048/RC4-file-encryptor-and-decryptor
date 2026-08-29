#ifndef RC4_H
#define RC4_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t S[256];     // State array
    uint8_t key[256];   // Key schedule
    size_t key_len;     // Key length
    int i;              // PRGA counter i
    int j;              // PRGA counter j
    int initialized;    // Flag to track if KSA has been run
} RC4_CTX;

// Initialize RC4 context with a key
void rc4_init(RC4_CTX *ctx, const uint8_t *key, size_t key_len);

// Generate keystream and XOR with data (encrypts/decrypts)
void rc4_crypt(RC4_CTX *ctx, const uint8_t *input, uint8_t *output, size_t length);

// Reset the PRGA state (for reusing the same key)
void rc4_reset(RC4_CTX *ctx);

#endif // RC4_H