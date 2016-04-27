#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>

#ifndef IVLEN
#define IVLEN 0x10
#endif

#ifndef EXTENSION
#define EXTENSION ".cbk"
#endif

struct CONTAINER_BLOCK
{
	char *container_iv;
	char *container_name;
	uint8_t container_size;
	uint8_t container_user_len;
	uint8_t container_pass_len;
	char *container_username;
	char *container_password;
};


int CONTAINER_BLOCK_init(struct CONTAINER_BLOCK *cb);
int CONTAINER_BLOCK_clear(struct CONTAINER_BLOCK *cb);
int CONTAINER_BLOCK_getsize(char *container_name);

int CONTAINER_BLOCK_read(struct CONTAINER_BLOCK *cb, char *container_name);
int CONTAINER_BLOCK_decrypt(unsigned char *blob, int blob_size, unsigned char *plaintext, unsigned char *key, unsigned char *iv);