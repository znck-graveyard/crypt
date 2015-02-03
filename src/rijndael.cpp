#include "rijndael.h"

#ifdef debug
#include <iostream>
#include <cstdio>
#endif

using namespace znck;

byte rijndael::sbox[256] = {0};

static inline byte calc_sbox(byte in) {
    byte out, s;
    if (0 == in) return 0x63;
    s = out = gmul_inv(in); // Calculate multiplicative inverse in 2^8
    for (int i = 0; i < 4; ++i) { // Affine transformation
        s = (s << 1) | (s >> 7);
        out ^= s;
    }
    out ^= 0x63;
    return out;
}

byte rijndael::get_sbox(byte in) {
    if (0x63 != sbox[0]) {
        for (int i = 0; i < 256; ++i) {
            sbox[i] = calc_sbox(i);
        }
    }

    return sbox[in];
}

byte rijndael::inv_sbox[256] = {0};

byte rijndael::get_inv_sbox(byte in) {
    if (0x52 != inv_sbox[0]) {
        for (int i = 0; i < 256; ++i) {
            inv_sbox[get_sbox(i)] = i;
        }
    }

    return inv_sbox[in];
}

byte rijndael::rcon[256] = {0};

static inline byte calc_rcon(byte in) {
    byte out = 1;
    if (0 == in) return 0x8d;
    while(in-- > 1) {
        out = gmul(out, 2);
    }
    return out;
}

byte rijndael::get_rcon(byte in) {
    if (0x8d != rcon[0]) {
        for (int i = 0; i <= 255; ++i) {
            rcon[i] = calc_rcon(i);
        }
    }

    return rcon[in];
}

byte * rijndael::expand_key(const byte key[16]) {
    byte *ex = new byte[176];
    // STEP 1: Copy 128 bits of encryption key
    for (int i = 0; i < 16; ++i) {
        ex[i] = key[i];
    }
    // STEP 2: Expansion
    int iteration = 1;
    byte t[4];
    for (int i = 16; i < 176; ) {
        for (int k = 0; k < 4; ++k) {
            t[k] = ex[i + k - 4];
        }
        if (i % 16 == 0) {
            conv_u32_to_u8(rotate_left(conv_u8_to_u32(t)), t);
            for (int k = 0; k < 4; ++k) {
                t[k] = get_sbox(t[k]);
            }
            t[0] = t[0] ^ get_rcon(iteration);
            ++iteration;
        }
        for (int k = 0; k < 4; ++k) {
            ex[i] = ex[i - 16] ^ t[k];
            ++i;
        }
    }

    return ex;
}

byte * rijndael::sub_bytes(byte *in) {
    for (int i = 0; i < 16; ++i) {
        in[i] = rijndael::get_inv_sbox(in[i]);
    }
    
    return in;
}

byte * rijndael::inv_sub_bytes(byte *in) {
    for (int i = 0; i < 16; ++i) {
        in[i] = rijndael::get_sbox(in[i]);
    }
    
    return in;
}

static byte * shift_row(byte *in) {
    byte temp;
    // Row 2
    temp  = in[1];
    in[1] = in[5];
    in[5] = in[9];
    in[9] = in[13];
    in[13] = temp;
    // Row 3
    temp   = in[2];
    in[ 2] = in[10];
    in[10] = temp;
    temp   = in[6];
    in[ 6] = in[14];
    in[14] = temp;
    // Row 4
    temp   = in[15];
    in[15] = in[11];
    in[11] = in[ 7];
    in[ 7] = in[ 3];
    in[ 3] = temp;
    return in;
}

static byte * inv_shift_row(byte *in) {
    byte temp;
    // Row 2
    temp   = in[13];
    in[13] = in[9];
    in[9]  = in[5];
    in[5]  = in[1];
    in[1]  = temp;
    // Row 3
    temp   = in[2];
    in[ 2] = in[10];
    in[10] = temp;
    temp   = in[6];
    in[ 6] = in[14];
    in[14] = temp;
    // Row 4
    temp   = in[5];
    in[5]  = in[7];
    in[7]  = in[11];
    in[11] = in[15];
    in[15] = temp;
    return in;
}

static byte * mix_columns(byte *in) {
    for (int i = 0; i < 16; i+=4) {
        conv_u32_to_u8(gmix_col(conv_u8_to_u32(in + i)), in + i);
    }
    return in;
}

static byte * inv_mix_columns(byte *in) {
    for (int i = 0; i < 16; i+=4) {
        conv_u32_to_u8(inv_gmix_col(conv_u8_to_u32(in + i)), in + i);
    }
    return in;
}

static byte * add_round_key(const byte *key, byte *in) {
    for (int i = 0; i < 16; ++i) {
        in[i] = key[i] ^ in[i];
    }
    return in;
}

byte * rijndael::encrypt(const byte *key, byte *state) {
    // Initial Round
    add_round_key(key, state);
    // Rounds
    for (int i = 1; i < 11; ++i) {
        sub_bytes(state);
        shift_row(state);
        if (10 != i) { mix_columns(state); }
        add_round_key(key + (i * 16), state);
    }

    return state;
}

byte * rijndael::decrypt(const byte *key, byte *state) {
    add_round_key(key + 160, state);
    for (int i = 10; i > 0; --i) {
        inv_shift_row(state);
        inv_sub_bytes(state);
        add_round_key(key + i * 16, state);
        if (1 != i) { inv_mix_columns(state); }
    }
    return state;
}

// ------------ Tests -----------

#ifdef unit_testing
#ifdef unit_test_rijndael_key_expansion

static byte hex_val(byte x) {
    if ('0' <= x && x <= '9') return x - '0';
    else if ('a' <= x && x <= 'f') return x - 'a' + 10;
    else throw "Invaild hex";
}

int main() {
    using namespace std;

    byte input[32], *ex;
    ex = new byte[176];
    int i;
    cin >> i;
    while(i-- && cin.good()) {
        cin >> input;
        byte key[16];
        for (int i = 0; i < 16; ++i) key[i] = hex_val(input[2 * i]) << 4 | hex_val(input[2 * i + 1]);
        rijndael::expand_key(key);
        for (int i = 0; i < 176; ++i) {
            if (i && i % 16 == 0) printf("\n");
            printf("%02x ", ex[i]);
        }
        printf("\n\n");
        if (cin.eof()) break;
    }
}
#endif
#ifdef unit_test_rijndael_shift_row
#include <cstdlib>
#include <cassert>
int main() {
    byte in[] = {0xa7, 0xf1, 0xd9, 0x2a, 0x82, 0xc8, 0xd8, 0xfe, 0x43, 0x4d, 0x98, 0x55, 0x8c, 0xe2, 0xb3, 0x47};
    byte out[] = {0xa7, 0xf1, 0xd9, 0x2a, 0xc8, 0xd8, 0xfe, 0x82, 0x98, 0x55, 0x43, 0x4d, 0x47, 0x8c, 0xe2, 0xb3};
    shift_row(in);
    for (int i = 0; i < 16; ++i) {
        assert(out[i] == in[i]);
    }
    printf("\n     --- passed\n");
}
#endif
#ifdef unit_test_rijndael_mix_columns
#include <cstdlib>
#include <cassert>
int main() {
    byte in[] = {   219, 242, 1, 198,
                     19,  10, 1, 198,
                     83,  34, 1, 198,
                     69,  92, 1, 198 };
    byte out[] = {  142, 159, 1, 198,
                     77, 220, 1, 198,
                    161,  88, 1, 198,
                    188, 157, 1, 198 };
    mix_columns(in);
    for (int i = 0; i < 16; ++i) {
        assert(out[i] == in[i]);
    }
    printf("\n     --- passed\n");
}
#endif
#endif