#ifndef BETTER_C_STD_PRETTIFY_PANIC_H_
#define BETTER_C_STD_PRETTIFY_PANIC_H_

#include <better_c_std/io/x_printf.h>
#include <better_c_std/io/out_stream.h>
#include <stdlib.h>

#define panic(...)                                                          \
  {                                                                         \
    OutStream err_stream = outstream_stderr();                              \
    x_sprintf(err_stream, "\n+-+-+-+-+-+-+-+-+-+-\n");                      \
    x_sprintf(err_stream, "Panic in file %s:%d, function %s: \n", __FILE__, \
              __LINE__, __func__);                                          \
    x_sprintf(err_stream, __VA_ARGS__);                                     \
    x_sprintf(err_stream, "\n");                                            \
    exit(-1);                                                               \
  }

#endif  // BETTER_C_STD_PRETTIFY_PANIC_H_