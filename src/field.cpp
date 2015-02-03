#include "field.h"

using namespace znck;

u32 znck::rotate_left(u32 in) {
    return ((in << 8) | (in >> 24)) & 0xffffffff;
}

u32 znck::rotate_right(u32 in) {
    return ((in >> 8) | (in << 24)) & 0xffffffff;
}

u8 znck::gmul(u8 a, u8 b) {
    byte p = 0, carry;
    for (byte i = 0; i < 8; ++i) {
        if (1 & b) { // If a is odd then gadd b to p
            p ^= a;
        }
        carry = a & 0x80; // Check if left shift will create a carry
        a <<= 1;
        if (carry) {
            a ^= 0x1B; // Replace x^8 with x^4 + x^3 + x + 1
        }
        b >>= 1;
    }
    return p;
}

u8 znck::gmul_inv(u8 in) {
    for (int i = 0; i <= 255; ++i) {
        if (1 == znck::gmul(in, i)) return i;
    }
    return 0;
}

u32 znck::gmix_col(u32 in) {
    byte a[4], r[4];
    conv_u32_to_u8(in, a);

    r[0] = gmul(a[0],2) ^ gmul(a[3],1) ^ gmul(a[2],1) ^ gmul(a[1],3);
    r[1] = gmul(a[1],2) ^ gmul(a[0],1) ^ gmul(a[3],1) ^ gmul(a[2],3);
    r[2] = gmul(a[2],2) ^ gmul(a[1],1) ^ gmul(a[0],1) ^ gmul(a[3],3);
    r[3] = gmul(a[3],2) ^ gmul(a[2],1) ^ gmul(a[1],1) ^ gmul(a[0],3);

    return conv_u8_to_u32(r);
}

u32 znck::inv_gmix_col(u32 in) {
    byte a[4], r[4];
    conv_u32_to_u8(in, a);

    r[0] = gmul(a[0],14) ^ gmul(a[3],11) ^ gmul(a[2],13) ^ gmul(a[1],9);
    r[1] = gmul(a[1],14) ^ gmul(a[0],11) ^ gmul(a[3],13) ^ gmul(a[2],9);
    r[2] = gmul(a[2],14) ^ gmul(a[1],11) ^ gmul(a[0],13) ^ gmul(a[3],9);
    r[3] = gmul(a[3],14) ^ gmul(a[2],11) ^ gmul(a[1],13) ^ gmul(a[0],9);

    return conv_u8_to_u32(r);
}

#ifdef unit_testing
#ifdef unit_test_gmix_col

#include <cstdio>
#include <cassert>

int main() {

    printf("0x09287f47 -> 0x%08lx ?= 0x529f16c2 \n", znck::gmix_col(0x09287f47));
    printf("0x6f746abf -> 0x%08lx ?= 0x978615ca \n", znck::gmix_col(0x6f746abf));
    printf("0x2c4a6204 -> 0x%08lx ?= 0xe01aae54 \n", znck::gmix_col(0x2c4a6204));
    printf("0xda08e3ee -> 0x%08lx ?= 0xba1a2659 \n", znck::gmix_col(0xda08e3ee));

    printf("0x096f2cda -> 0x%08lx ?= 0x5297e0ba \n", znck::gmix_col(0x096f2cda));
    printf("0x28744a08 -> 0x%08lx ?= 0x9f861a1a \n", znck::gmix_col(0x28744a08));
    printf("0x7f6a62e3 -> 0x%08lx ?= 0x1615ae26 \n", znck::gmix_col(0x7f6a62e3));
    printf("0x47bf04ee -> 0x%08lx ?= 0xc2ca5459 \n", znck::gmix_col(0x47bf04ee));

    printf("0xdb135345 -> 0x%08lx ?= 0x8e4da1bc \n", znck::gmix_col(0xdb135345));
    printf("0xf20a225c -> 0x%08lx ?= 0x9fdc589d \n", znck::gmix_col(0xf20a225c));
    printf("0x01010101 -> 0x%08lx ?= 0x01010101 \n", znck::gmix_col(0x01010101));
    printf("0xd4d4d4d5 -> 0x%08lx ?= 0xd5d5d7d6 \n", znck::gmix_col(0xd4d4d4d5));
    printf("0x2d26314c -> 0x%08lx ?= 0x4d7ebdf8 \n", znck::gmix_col(0x2d26314c));

    printf("0xf8 -> 0x%02x ?= 0x3a \n", znck::gmul_inv(0xf8));
    printf("0xb8 -> 0x%02x ?= 0x3a \n", znck::gmul_inv(0xb8));
    printf("0x50 -> 0x%02x ?= 0x3a \n", znck::gmul_inv(0x50));
    printf("0xfd -> 0x%02x ?= 0x3a \n", znck::gmul_inv(0xfd));
    assert(0xbc == gmul(0xbc, 1));
    printf("--passed\n");
    return 0;
}
#endif
#endif
