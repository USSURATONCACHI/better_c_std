#ifndef BETTER_C_STD_STRING_STR_H_
#define BETTER_C_STD_STRING_STR_H_

#include <stdbool.h>
#include <better_c_std/result.h>

#include <better_c_std/string/str_struct.h>

BcstdStr BcstdStr_literal(const char* literal);
BcstdStr BcstdStr_borrow(const BcstdStr* from);
BcstdStr BcstdStr_owned(const char* format, ...);

BcstdStr BcstdStr_raw_owned(char* text);
BcstdStr BcstdStr_clone(const BcstdStr* source);
void BcstdStr_free(BcstdStr s);
void BcstdStr_free_p(BcstdStr* s);

BcstdStr read_file_to_str(const char* filename);

// 
typedef STRUCT_RESULT(BcstdStr, BcstdStr) BcstdStrResult;
void BcstdStr_result_free(BcstdStrResult self);

#define BcstdStrErr(e) \
  (BcstdStrResult) { .is_ok = false, .data = e }

#define BcstdStrOk(e) \
  (BcstdStrResult) { .is_ok = true, .data = e }

#endif  // BETTER_C_STD_STRING_STR_H_