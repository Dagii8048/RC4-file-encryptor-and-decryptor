CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -pedantic
TARGETS = rc4_encryptor rc4_decryptor
OBJS = rc4.o

all: $(TARGETS)

rc4_encryptor: rc4_encryptor.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

rc4_decryptor: rc4_decryptor.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

rc4_encryptor.o: rc4_encryptor.c rc4.h
	$(CC) $(CFLAGS) -c $<

rc4_decryptor.o: rc4_decryptor.c rc4.h
	$(CC) $(CFLAGS) -c $<

rc4.o: rc4.c rc4.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) *.o

test:
	@echo "=== Testing RC4 File Encryption ==="
	@echo "This is a test file for RC4 encryption." > file.txt
	@echo "[1] Original file contents:"
	@cat file.txt
	@echo ""
	@echo "[2] Encrypting with key 'TestKey123'..."
	@./rc4_encryptor TestKey123
	@echo "[3] Encrypted file (binary data):"
	@xxd file.txt | head -5
	@echo ""
	@echo "[4] Decrypting with key 'TestKey123'..."
	@./rc4_decryptor TestKey123
	@echo "[5] Decrypted file contents:"
	@cat file.txt
	@echo ""
	@echo "=== Test Complete ==="

.PHONY: all clean test