#include <better_c_std/time.h>
#include <better_c_std/prettify/assert.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define NANOS_IN_A_SECOND (1000ULL * 1000ULL * 1000ULL)

#ifdef _WIN32
    BcstdTime BcstdTime_now_unix() {
        FILETIME ft;
        GetSystemTimePreciseAsFileTime(&ft);

        // Convert FILETIME (100-nanosecond intervals since Jan 1, 1601) to Unix epoch
        uint64_t t_100ns = (((uint64_t)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
        
        //               (  microseconds ) -> millis -> seconds
        uint64_t t_sec = (t_100ns / 10ULL) / 1000ULL / 1000ULL;

        uint64_t how_many_100ns_intervals_in_1_sec = NANOS_IN_A_SECOND / 100ULL;
        uint64_t t_nanos = (t_100ns % how_many_100ns_intervals_in_1_sec) * 100ULL;

        // Number of leap years (1601 - 1970) = 89
        uint64_t num_leap_years = 89;
        uint64_t unix_diff_days = (1970 - 1601) * 365 + num_leap_years; // each leap year is 1 more day
        uint64_t unix_diff_secs = unix_diff_days * 24 * 60 * 60; // days -> hrs -> mins -> secs

        // Its pretty reasonable to assume, that this code will be never ran before 1970, Jan 1.
        // But if this code time-travels, or OS gives us some dogwater data - we won't fail.
        // Behaviour is not specified, but it will pretty much just overflow and become 
        // approximately (i did not account for every 100-year-non-leap-year):
        // Year 584542047691, Aug 21, 01:00:16 (hours:minutes:seconds) up to and overflowing to Jan 1, 1970.
        t_sec -= unix_diff_secs; 

        currentTime.seconds = t / 10000000ULL;
        currentTime.nanos = (t % 10000000ULL) * 100ULL;
        return (BcstdTime) {
            .seconds = t_sec,
            .nanos = t_nanos;
        };
    }
#else
    BcstdTime BcstdTime_now_unix() {
        // In linux its actually really simple
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        return (BcstdTime) {
            .seconds = ts.tv_sec,
            .nanos = ts.tv_nsec,
        };
    }
#endif

BcstdTime BcstdTime_normalize(BcstdTime t) {
    if (t.nanos >= NANOS_IN_A_SECOND) {
        t.seconds += t.nanos / NANOS_IN_A_SECOND;
        t.nanos = t.nanos % NANOS_IN_A_SECOND;
    }
    return t;
}

BcstdTime BcstdTime_diff(BcstdTime lhs, BcstdTime rhs) {
    lhs = BcstdTime_normalize(lhs);
    rhs = BcstdTime_normalize(rhs);
    int64_t nanos_l = lhs.nanos;
    int64_t nanos_r = rhs.nanos;
    int64_t nanos_diff = nanos_l - nanos_r;

    BcstdTime diff = {
        .seconds = lhs.seconds - rhs.seconds,
        .nanos = 0
    };

    if (nanos_diff >= 0) {
        diff.nanos = nanos_diff;
    } else {
        // No need for mod operator, we won't have more than 1-second overflow
        diff.nanos = nanos_diff + NANOS_IN_A_SECOND;
        diff.seconds -= 1;
    }

    return diff;
}
BcstdTime BcstdTime_sum(BcstdTime lhs, BcstdTime rhs) {
    BcstdTime sum = {
        .seconds = lhs.seconds + rhs.seconds,
        .nanos = lhs.nanos + rhs.nanos,
    };
    return BcstdTime_normalize(sum);
}

long double BcstdTime_to_ldouble_secs_imprecise(BcstdTime t) {
    long double secs = t.seconds;
    long double nanos = t.nanos;
    nanos /= (long double) NANOS_IN_A_SECOND;
    return secs + nanos;
}