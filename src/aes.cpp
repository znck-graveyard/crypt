#include "aes.h"
#include <cstdio>
#include <cstdlib>

using namespace std;
using namespace znck;

// TODO make it generic
static byte *add_pad_pkcs5(byte * in, size_t &len, size_t block = 128) {
	block /= 8;
	byte t = block - (len % block);
	byte p = t << 4 | t;
	realloc(in, len + t);
	while (t) {
		in[len++] = p;
		--t;
	}
	return in;
}

// TODO make it generic
static byte *ctr_encrypt(byte *key, byte *message, size_t len, byte *IV) {
	return message;
}

#include <cstdio>

static inline void print(byte * in) {
	for (int i = 0; i < 16; ++i)
	{
		printf("%02x", in[i]);
	}
	printf("\n");
}

// TODO make it generic
static byte *ctr_decrypt(byte *key, byte *message, size_t &len) {
	byte c = 0;
	byte *IV = message;
	for (int i = 16; i < 16 + len; i += 16) {
		rijndael::decrypt(key, message + i);
		for (int j = 0; j < 16; ++j) {
			message[i + j] ^= IV[j];
		}
		message[i + 15] ^= c;
		++c;
	}
	return message + 16;
}

static byte *cbc_decrypt(byte *key, byte *message, size_t &len) {
	byte c[32] = {0};
	// Copy IV
	strncpy((char *)c, (const char *)(message), 16);
	for (int i = 16; i < 16 + len; i += 16) {
		// Copy current block
		strncpy((char *)(c + 16), (const char *)message + i, 16);
		rijndael::decrypt(key, message + i);
		for (int j = 0; j < 16; ++j) {
			message[i + j] ^= c[j];
		}
		// Queue current cipher block for next chaining
		strncpy((char *)c, (const char *)(c + 16), 16);
	}

	len -= message[len + 15];
	return message + 16;
}

static byte *ecb_encrypt(byte *key, byte *message, size_t len) {
	for (int i = 0; i < len; i += 16) {
		rijndael::encrypt(key, message + i);
	}
	return message;
}

byte * AES::encrypt(const byte *k, byte *message, size_t &len, byte mode, byte *_IV) {
	if (mode & AES_PAD_PKCS5) {
		add_pad_pkcs5(message, len);
	}

	byte *key = new byte[176];
	key = rijndael::expand_key(k);

	if (mode & AES_MODE_ECB) {
		return ecb_encrypt(key, message, len);
	}

	throw "Unsupported mode";
}

static byte *ecb_decrypt(byte *key, byte *message, size_t len) {
	for (int i = 0; i < len; i += 16) {
		rijndael::decrypt(key, message + i);
	}
	return message;
}

byte * AES::decrypt(const byte *k, byte *message, size_t &len, byte mode, byte *_IV) {
	byte *key;
	key = rijndael::expand_key(k);

	if (mode & AES_MODE_ECB) {
		return ecb_decrypt(key, message, len);
	} else if (mode & AES_MODE_CTR) {
		len -= 16;
		return ctr_decrypt(key, message, len);
	} else if(mode & AES_MODE_CBC) {
		len -= 16;
		return cbc_decrypt(key, message, len);
	}

	throw "Unsupported mode";
}