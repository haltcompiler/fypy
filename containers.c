#include "containers.h"

int CONTAINER_BLOCK_init(struct CONTAINER_BLOCK *cb)
{
	memset(&cb, 0x00, sizeof(struct CONTAINER_BLOCK));
	return 1;
}

int CONTAINER_BLOCK_clear(struct CONTAINER_BLOCK *cb)
{
	memset(&cb, 0x00, sizeof(struct CONTAINER_BLOCK));
	return 1;
}

int CONTAINER_BLOCK_getsize(char *container_name)
{
	struct stat st;
	stat(container_name, &st);
	return st.st_size;
}

int CONTAINER_BLOCK_decrypt(unsigned char *blob, int blob_size, unsigned char *plaintext, unsigned char *key, unsigned char *iv)
{
	EVP_CIPHER_CTX *ctx;
	int len, plaintext_len;

	if(!(ctx = EVP_CIPHER_CTX_new())) return 1;
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) return 1;
	if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, blob, blob_size)) return 1;
  
	plaintext_len = len;

	if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) return 1;
  
	plaintext_len += len;

	EVP_CIPHER_CTX_free(ctx);
	return plaintext_len;
}

int CONTAINER_BLOCK_read(struct CONTAINER_BLOCK *cb, char *container_name)
{
	uint8_t ulen, plen;
	int originalStreamSize, fileDescContainer, blob_size, plaintext_len;
	char iv[17];
	char *blob, *plaintext, *container, *username, *password;

	unsigned char *key = (unsigned char *)"0123456789012345";

	// Getting file size for plaintext 
	size_t container_size = CONTAINER_BLOCK_getsize(container_name);
	container = (char *)malloc( container_size );
	
	// Reading all encrypted data.
	fileDescContainer = open(container_name, O_RDONLY);
	read(fileDescContainer, container, container_size);
	
	// Allocated space for plaintext.
	originalStreamSize = *(container);
	plaintext = (char *)malloc(originalStreamSize + 1);

	// Extracted IV for the data.
	memcpy(iv, (container + 1), IVLEN);
	iv[16] = '\0';

	// Reading blob for decryption
	blob_size = container_size- IVLEN - 1;
	blob = (char *)malloc( blob_size);

	memcpy(blob, (container + 1 + IVLEN), (blob_size));

	// Decryption starting here
	plaintext_len = CONTAINER_BLOCK_decrypt(blob, blob_size, plaintext, key, iv);
	plaintext[plaintext_len] = '\0';

	// Extracting username and password
	ulen = plaintext[0];
	plen = container_size - ulen - 2;
	username = (char *) malloc(ulen + 1);
	memcpy(username, (plaintext + 2), ulen);
	username[ulen] = '\0';
	password = (char *) malloc(plen + 1);
	memcpy(password, (plaintext + 2 + ulen), container_size - ulen - 2);
	password[plen] = '\0';

	// Loading Container state structure.
	cb->container_iv = iv;
	cb->container_name = container_name;
	cb->container_size = container_size;
	cb->container_user_len = ulen;
	cb->container_pass_len = plen;
	cb->container_username = username;
	cb->container_password = password;
	
	close(fileDescContainer);
	free(blob);
	free(container);
	memset(plaintext, 0x00, plaintext_len);
	free(plaintext);
}