#ifndef BETTER_C_STD_STRING_STR_T_RAW_H_
#define BETTER_C_STD_STRING_STR_T_RAW_H_

#include <stdbool.h>

// This can be either string reference, or allocated string
// To be able to operate both, this type is used

typedef struct {
  const char* string;
  bool is_owned;
} str_t;

#endif  // BETTER_C_STD_STRING_STR_T_RAW_H_