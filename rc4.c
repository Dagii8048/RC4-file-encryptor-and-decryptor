#include "rc4.h"
#include <string.h>

void rc4_init(RC4_CTX *ctx, const uint8_t *key, size_t key_len) {
    int i, j;
    uint8_t tmp;
    
    // Store the key
    ctx->key_len = key_len;
    memcpy(ctx->key, key, key_len);
    
    // Key Scheduling Algorithm (KSA)
    // Initialize S with identity permutation
    for (i = 0; i < 256; i++) {
        ctx->S[i] = i;
    }
    
    // Scramble S using the key
    j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + ctx->S[i] + key[i % key_len]) & 0xFF;  // mod 256 using bitmask
        // Swap S[i] and S[j]
        tmp = ctx->S[i];
        ctx->S[i] = ctx->S[j];
        ctx->S[j] = tmp;
    }
    
    // Reset PRGA counters
    ctx->i = 0;
    ctx->j = 0;
    ctx->initialized = 1;
}

void rc4_reset(RC4_CTX *ctx) {
    // Re-initialize the cipher with the stored key
    if (ctx->key_len > 0) {
        rc4_init(ctx, ctx->key, ctx->key_len);
    }
}

void rc4_crypt(RC4_CTX *ctx, const uint8_t *input, uint8_t *output, size_t length) {
    int i, j;
    uint8_t tmp, keystream_byte;
    size_t n;
    
    if (!ctx->initialized) {
        return;  // Error: not initialized
    }
    
    // Use local variables for speed
    i = ctx->i;
    j = ctx->j;
    
    for (n = 0; n < length; n++) {
        // PRGA - Generate keystream byte
        i = (i + 1) & 0xFF;
        j = (j + ctx->S[i]) & 0xFF;
        
        // Swap S[i] and S[j]
        tmp = ctx->S[i];
        ctx->S[i] = ctx->S[j];
        ctx->S[j] = tmp;
        
        // Generate keystream byte
        keystream_byte = ctx->S[(ctx->S[i] + ctx->S[j]) & 0xFF];
        
        // XOR with input byte
        output[n] = input[n] ^ keystream_byte;
    }
    
    // Save PRGA state
    ctx->i = i;
    ctx->j = j;
}