#ifndef BETTER_C_STD_TIME_H_
#define BETTER_C_STD_TIME_H_

#include <stdint.h>
#include <stdbool.h>

// Unlike some, this time type is well-defined and platform-independent.
// I look at you, libc <_< .
typedef struct {
    uint64_t seconds;
    uint64_t nanos;
} BcstdTime;

BcstdTime BcstdTime_now_unix();

BcstdTime BcstdTime_from_secs(uint64_t secs);
BcstdTime BcstdTime_from_millis(uint64_t millis);
BcstdTime BcstdTime_from_micros(uint64_t micros);
BcstdTime BcstdTime_from_nanos(uint64_t nanos);

// Checks if `BcstdTime` has too much nanos (>=1s equivalent) and converts that overflow into seconds
BcstdTime BcstdTime_normalize(BcstdTime t);

BcstdTime BcstdTime_diff(BcstdTime lhs, BcstdTime rhs);
BcstdTime BcstdTime_sum(BcstdTime lhs, BcstdTime rhs);

bool BcstdTime_is_greater(BcstdTime lhs, BcstdTime rhs);
bool BcstdTime_is_less(BcstdTime lhs, BcstdTime rhs);

long double BcstdTime_to_ldouble_secs_imprecise(BcstdTime t);

#endif // BETTER_C_STD_TIME_H_