#ifndef _field_h_
#define _field_h_

#include "types.h"

namespace znck {
    u32 rotate_left(u32);
    u32 rotate_right(u32);
    u8 gmul(u8, u8);
    u8 gmul_inv(u8);
    u32 gmix_col(u32);
    u32 inv_gmix_col(u32);
};

#endif
