#ifndef BETTER_C_STD_IO_OUT_STREAM_H_
#define BETTER_C_STD_IO_OUT_STREAM_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include <better_c_std/string/str.h>

typedef struct OutStreamVtable {
    int (*putc)(void* self, int);
    int (*puts)(void* self, const char* str);
    int (*put_slice)(void* self, const char* str, size_t length);
    int (*write)(void* self, const void* data, size_t size);

    size_t (*get_available_size)(void* self);
    BcstdStr (*description)(void* self);
} OutStreamVtable;

// OutStream is a fat pointer to some object.
typedef struct OutStream {
    void* data;
    const OutStreamVtable* vtable;
} OutStream;

OutStream OutStream_from_file(FILE* file);
OutStream OutStream_stdout();
OutStream OutStream_stderr();

// Simple wrappers for calls
static inline int OutStream_putc(OutStream self, int c) {
    if (self.vtable && self.vtable->putc)
        return self.vtable->putc(self.data, c);
    return 0;
}
static inline int OutStream_puts(OutStream self, const char* string) {
    if (self.vtable && self.vtable->puts)
        return self.vtable->puts(self.data, string);
    return 0;
}
static inline int OutStream_put_slice(OutStream self, const char* string, size_t length) {
    if (self.vtable && self.vtable->put_slice)
        return self.vtable->put_slice(self.data, string, length);
    return 0;
}
static inline size_t OutStream_available_space(OutStream self) {
    if (self.vtable && self.vtable->get_available_size)
        return self.vtable->get_available_size(self.data);
    return SIZE_MAX;
}
static inline BcstdStr OutStream_description(OutStream self) {
    if (self.vtable && self.vtable->description)
        return self.vtable->description(self.data);
    return BcstdStr_literal("<null OutStream>");
}
static inline int OutStream_write(OutStream self, void* data, size_t size) {
    if (self.vtable && self.vtable->write)
        return self.vtable->write(self.data, data, size);
    return 0;
}

#endif  // BETTER_C_STD_IO_OUT_STREAM_H_