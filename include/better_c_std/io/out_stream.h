#ifndef BETTER_C_STD_IO_OUT_STREAM_H_
#define BETTER_C_STD_IO_OUT_STREAM_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include <better_c_std/string/str_t_raw.h>

typedef struct OutStreamVtable {
    int (*putc)(void* tthis, int);
    int (*puts)(void* tthis, const char* str);
    int (*put_slice)(void* tthis, const char* str, size_t length);

    size_t (*get_available_size)(void* tthis);
    str_t (*description)(void* tthis);
} OutStreamVtable;

typedef struct OutStream {
    void* data;
    const OutStreamVtable* vtable;
} OutStream;

int outstream_putc(int c, OutStream tthis);
int outstream_puts(const char* string, OutStream tthis);
int outstream_put_slice(const char* string, size_t length, OutStream tthis);
size_t outstream_available_space(OutStream tthis);
str_t outstream_description(OutStream tthis);

void outstream_x_vprintf(OutStream tthis, va_list list);
void outstream_x_printf(OutStream tthis, ...);

OutStream outstream_from_file(FILE* file);
OutStream outstream_stdout();
OutStream outstream_stderr();

typedef struct BufferOutStream {
    char* buffer;
    size_t length;
    size_t pos;
} BufferOutStream;
OutStream outstream_from_buffer(BufferOutStream* b);

#endif  // BETTER_C_STD_IO_OUT_STREAM_H_