#include "types.h"

using namespace znck;

u32 znck::conv_u8_to_u32(u8 in[4]) {
	return ((u32)in[0] << 24) ^ ((u32)in[1] << 16) ^ ((u32)in[2] <<  8) ^ ((u32)in[3]);
}

u8 * znck::conv_u32_to_u8(u32 in, u8 *out) {
	out[0] = 0xff & (u8)(in >> 24);
	out[1] = 0xff & (u8)(in >> 16);
	out[2] = 0xff & (u8)(in >>  8);
	out[3] = 0xff & (u8)(in      );

	return out;
}