#ifndef BETTER_C_STD_PRETTIFY_PANIC_H_
#define BETTER_C_STD_PRETTIFY_PANIC_H_

#include <better_c_std/io/out_stream.h>
#include <better_c_std/io/x_printf.h>
#include <better_c_std/prettify/term_colors.h>
#include <stdlib.h>

#define panic(...)                                                             \
    {                                                                          \
        OutStream err_stream = OutStream_stderr();                             \
        x_sprintf(err_stream, "\n+-+-+-+-+-+-+-+-+-+-\n");                     \
        x_sprintf(err_stream,                                                  \
                  TERM_COLOR_RED                                               \
                  "PANIC" TERM_COLOR_RESET " in file " TERM_COLOR_GRAY         \
                  "%s:%d" TERM_COLOR_RESET ", function " TERM_COLOR_BLUE       \
                  "%s" TERM_COLOR_RESET ": \n",                                \
                  __FILE__, __LINE__, __func__);                               \
        x_sprintf(err_stream, __VA_ARGS__);                                    \
        x_sprintf(err_stream, "\n");                                           \
        exit(-1);                                                              \
    }

#endif // BETTER_C_STD_PRETTIFY_PANIC_H_