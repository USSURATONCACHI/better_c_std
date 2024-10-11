#ifndef BETTER_C_STD_STRING_STR_SLICE_H_
#define BETTER_C_STD_STRING_STR_SLICE_H_

#include <stdbool.h>
#include <stddef.h>
#include <better_c_std/string/str_struct.h>

typedef struct StrSlice {
  const char* start;
  size_t length;
} StrSlice;

#define StrSlice_NULL ((StrSlice) { NULL, 0 })

bool BcstdStrSlice_eq_ccp(StrSlice slice, const char* string);
bool BcstdStrSlice_eq(StrSlice a, StrSlice b);
BcstdStr BcstdStrSlice_to_owned(StrSlice slice);
StrSlice BcstdStrSlice_from_string(const char* str);
StrSlice BcstdStrSlice_from_BcstdStr(const BcstdStr* str);

#endif  // BETTER_C_STD_STRING_STR_SLICE_H_