#ifndef BETTER_C_STD_STRING_STR_SLICE_H_
#define BETTER_C_STD_STRING_STR_SLICE_H_

#include <stdbool.h>
#include <better_c_std/string/str_t_raw.h>

typedef struct StrSlice {
  const char* start;
  int length;
} StrSlice;

bool str_slice_eq_ccp(StrSlice slice, const char* string);
bool str_slice_eq(StrSlice a, StrSlice b);
str_t str_slice_to_owned(StrSlice slice);
StrSlice str_slice_from_string(const char* str);
StrSlice str_slice_from_str_t(const str_t* str);

#endif  // BETTER_C_STD_STRING_STR_SLICE_H_