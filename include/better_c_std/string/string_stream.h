#ifndef BETTER_C_STD_STRING_STRING_STREAM_H_
#define BETTER_C_STD_STRING_STRING_STREAM_H_

#include <stdint.h>

#include <better_c_std/io.h>

typedef struct BcstdStringStream {
  char* buffer;
  size_t capacity;
  size_t length;
} BcstdStringStream;

void BcstdStringStream_free(BcstdStringStream self);
BcstdStringStream BcstdStringStream_create();
BcstdStringStream BcstdStringStream_clone(const BcstdStringStream* source);
void BcstdStringStream_print(const BcstdStringStream* self, OutStream out);

OutStream BcstdStringStream_stream(BcstdStringStream* self);
char* BcstdStringStream_collect(BcstdStringStream self);
BcstdStr BcstdStringStream_to_BcstdStr(BcstdStringStream self);

#endif  // BETTER_C_STD_STRING_STRING_STREAM_H_