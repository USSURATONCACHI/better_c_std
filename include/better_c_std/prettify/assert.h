#ifndef BETTER_C_STD_PRETTIFY_ASSERT_H_
#define BETTER_C_STD_PRETTIFY_ASSERT_H_

#include <better_c_std/prettify/panic.h>

#define assert_alloc(ptr) \
  if (ptr == NULL) panic("Failed to allocate memory");

#define assert_m(cond) \
  if (!(cond)) panic("Assertion failed\n");

#endif  // BETTER_C_STD_PRETTIFY_ASSERT_H_