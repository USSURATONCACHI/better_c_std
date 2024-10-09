#ifndef BETTER_C_STD_TIME_H_
#define BETTER_C_STD_TIME_H_

#include <stdint.h>

// Unlike some, this time type is well-defined and platform-independent.
// I look at you, C stdlib <_< .
typedef struct {
    uint64_t seconds;
    uint64_t nanos;
} BcstdTime;

BcstdTime BcstdTime_now_unix();

// Checks if `BcstdTime` has too much nanos (>1s equivalent) and converts that overflow into seconds
BcstdTime BcstdTime_normalize(BcstdTime t);

BcstdTime BcstdTime_diff(BcstdTime lhs, BcstdTime rhs);
BcstdTime BcstdTime_sum(BcstdTime lhs, BcstdTime rhs);

long double BcstdTime_to_ldouble_secs_imprecise(BcstdTime t);

#endif // BETTER_C_STD_TIME_H_