#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rc4.h"

#define BUFFER_SIZE 4096

void print_usage(const char *program_name) {
    printf("RC4 File Encryptor\n");
    printf("Usage: %s <key>\n", program_name);
    printf("\nThis program encrypts 'file.txt' using RC4 with the provided key.\n");
    printf("The original file is overwritten with encrypted data.\n");
    printf("\nArguments:\n");
    printf("  key    Secret key for encryption (string)\n");
    printf("\nExample:\n");
    printf("  %s MySecretKey123\n", program_name);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    RC4_CTX ctx;
    uint8_t *buffer;
    uint8_t *encrypted;
    long file_size;
    size_t bytes_read;
    const char *key_string;
    size_t key_len;
    const char *filename = "file.txt";
    
    // Check command line arguments
    if (argc != 2) {
        printf("Error: Key required\n\n");
        print_usage(argv[0]);
        return 1;
    }
    
    key_string = argv[1];
    key_len = strlen(key_string);
    
    if (key_len == 0) {
        printf("Error: Key cannot be empty\n");
        return 1;
    }
    
    printf("=== RC4 File Encryptor ===\n");
    printf("[*] Key: %s (length: %zu bytes)\n", key_string, key_len);
    printf("[*] Target file: %s\n", filename);
    
    // Open file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: Cannot open file '%s'\n", filename);
        printf("Please ensure 'file.txt' exists in the current directory.\n");
        return 1;
    }
    
    // Get file size
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    printf("[*] File size: %ld bytes\n", file_size);
    
    if (file_size == 0) {
        printf("Warning: File is empty. Nothing to encrypt.\n");
        fclose(fp);
        return 0;
    }
    
    // Allocate memory for file contents
    buffer = (uint8_t*)malloc(file_size);
    if (!buffer) {
        printf("Error: Memory allocation failed\n");
        fclose(fp);
        return 1;
    }
    
    // Read file contents
    bytes_read = fread(buffer, 1, file_size, fp);
    if (bytes_read != file_size) {
        printf("Error: Failed to read entire file\n");
        free(buffer);
        fclose(fp);
        return 1;
    }
    fclose(fp);
    
    // Allocate memory for encrypted data
    encrypted = (uint8_t*)malloc(file_size);
    if (!encrypted) {
        printf("Error: Memory allocation failed\n");
        free(buffer);
        return 1;
    }
    
    // Initialize RC4 with the key
    rc4_init(&ctx, (const uint8_t*)key_string, key_len);
    
    // Encrypt the data
    printf("[*] Encrypting...\n");
    rc4_crypt(&ctx, buffer, encrypted, file_size);
    
    // Open file for writing (overwrite)
    fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: Cannot write to file '%s'\n", filename);
        free(buffer);
        free(encrypted);
        return 1;
    }
    
    // Write encrypted data back to file
    fwrite(encrypted, 1, file_size, fp);
    fclose(fp);
    
    printf("[*] Encryption complete!\n");
    printf("[*] '%s' has been encrypted and overwritten.\n", filename);
    printf("[*] Use the decryptor with the same key to restore.\n");
    
    // Clean up
    free(buffer);
    free(encrypted);
    
    return 0;
}