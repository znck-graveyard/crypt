#ifndef _types_h_
#define _types_h_

namespace znck {
	typedef unsigned char u8;
    typedef unsigned long u32;

    typedef u8 byte;

    u32 conv_u8_to_u32(u8 *);
    u8 * conv_u32_to_u8(u32, u8 *);
};

#endif