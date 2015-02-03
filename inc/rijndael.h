#ifndef _cipher_h_
#define _cipher_h_

#include "types.h"
#include "field.h"

namespace znck {
    class rijndael{
        private:
            static byte sbox[256];
            static byte get_sbox(byte);

            static byte inv_sbox[256];
            static byte get_inv_sbox(byte);

            static byte rcon[256];
            static byte get_rcon(byte);
            static byte * sub_bytes(byte *);
            static byte * inv_sub_bytes(byte *);

        public:
            static byte * expand_key(const byte *);
            static byte * encrypt(const byte *, byte *);
            static byte * decrypt(const byte *, byte *);

#ifdef unit_test_rijndael_encrypt

#endif 

#ifdef unit_test_rijndael_sbox
            static void test_sbox();
#endif

#ifdef unit_test_rijndael_rcon
            static void test_rcon();
#endif
    };
}

#endif
