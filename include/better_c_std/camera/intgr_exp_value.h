#ifndef BETTER_C_STD_CAMERA_INTGR_EXP_VALUE_H_
#define BETTER_C_STD_CAMERA_INTGR_EXP_VALUE_H_

#include <better_c_std/time.h>
#include <better_c_std/prettify/debug.h>
#include <math.h>

// Mathematical abstraction over exponential velocities
// v(t) = v0 * exponent^t        ; '^' = raise to power
// x(t) = integral(0, t, v(g)dg) + x0;
typedef struct {
    long double x0;
    long double v0;
    long double exponent;
    BcstdTime start_time;
} BcstdIntgrExpValue;

// does not worth to be put into a separate translation unit. Let it be static
static inline BcstdIntgrExpValue BcstdIntgrExpValue_from_exponent(long double exp) {
    return (BcstdIntgrExpValue) {
        .x0 = 0.0,
        .v0 = 0.0,
        .exponent = exp,
        .start_time = (BcstdTime){
            .seconds = 0,
            .nanos = 0,
        },
    }; 
}

static inline long double BcstdIntgrExpValue_get_vel(BcstdIntgrExpValue val, BcstdTime now) {
    BcstdTime time_diff = BcstdTime_diff(now, val.start_time);
    long double t = BcstdTime_to_ldouble_secs_imprecise(time_diff);
    return val.v0 * powl(val.exponent, t);
}

static inline long double BcstdIntgrExpValue_get_pos(BcstdIntgrExpValue val, BcstdTime now) {
    BcstdTime time_diff = BcstdTime_diff(now, val.start_time);
    long double t = BcstdTime_to_ldouble_secs_imprecise(time_diff);
    
    return val.v0 * (powl(val.exponent, t) - 1.0) / logl(val.exponent) + val.x0;
}

static inline long double BcstdIntgrExpValue_get_pos_eventual(BcstdIntgrExpValue val) {
    return val.v0 * ((long double) -1.0) / logl(val.exponent) + val.x0;
}

static inline BcstdIntgrExpValue BcstdIntgrExpValue_update(BcstdIntgrExpValue val, BcstdTime now) {
    long double x = BcstdIntgrExpValue_get_pos(val, now);
    long double v = BcstdIntgrExpValue_get_vel(val, now);

    return (BcstdIntgrExpValue) {
        .x0 = x,
        .v0 = v,
        .exponent = val.exponent,
        .start_time = now,
    };
}



#endif // BETTER_C_STD_CAMERA_INTGR_EXP_VALUE_H_