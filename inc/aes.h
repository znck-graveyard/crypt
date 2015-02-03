#ifndef _aes_h_
#define _aes_h_

#include "rijndael.h"
#include <cstdio>
#include <cstring>

using namespace std;

#define AES_MODE_ECB  0x01
#define AES_MODE_CTR  0x02
#define AES_MODE_CBC  0x04

#define AES_PAD_PKCS5 0x80

namespace znck {
	class AES{
	public:
		static byte *encrypt(const byte *, byte *, size_t &, byte, byte *_IV = NULL);
		static byte *decrypt(const byte *, byte *, size_t &, byte, byte *_IV = NULL);
	};
}

#endif