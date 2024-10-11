#include <better_c_std/io/out_stream.h>
#include <better_c_std/string/str.h>
#include <better_c_std/prettify.h>

#include <limits.h>
#include <stdio.h>

static int putc_file(FILE* self, int c) { return putc(c, self); }
static int puts_file(FILE* self, const char* str) { return fputs(str, self); }
static int put_slice_file(FILE* self, const char* str, size_t length) {
    fprintf(self, "%.*s", (int)length, str);
    return '\n';
}
static size_t get_size_vtable(FILE* self) {
    unused(self);
    return SIZE_MAX;
}
static BcstdStr description_file(FILE* self) {
    unused(self);
    return BcstdStr_literal("FILE*");
}
static BcstdStr description_stdout(FILE* self) {
    unused(self);
    return BcstdStr_literal("stdout");
}
static BcstdStr description_stderr(FILE* self) {
    unused(self);
    return BcstdStr_literal("stderr");
}

OutStream OutStream_from_file(FILE* file) {
    static const OutStreamVtable FILE_VTABLE = {
            .putc               = (void*)putc_file,
            .puts               = (void*)puts_file,
            .put_slice          = (void*)put_slice_file,
            .get_available_size = (void*)get_size_vtable,
            .description        = (void*)description_file
        };

    return (OutStream){.data = file, .vtable = &FILE_VTABLE};
}

OutStream OutStream_stdout() {
    static const OutStreamVtable STDOUT_VTABLE = {
            .putc               = (void*)putc_file,
            .puts               = (void*)puts_file,
            .put_slice          = (void*)put_slice_file,
            .get_available_size = (void*)get_size_vtable,
            .description        = (void*)description_stdout, // self one is different
        };

    return (OutStream){.data = stdout, .vtable = &STDOUT_VTABLE};
}
OutStream OutStream_stderr() {
    static const OutStreamVtable STDERR_VTABLE = {
            .putc               = (void*)putc_file,
            .puts               = (void*)puts_file,
            .put_slice          = (void*)put_slice_file,
            .get_available_size = (void*)get_size_vtable,
            .description        = (void*)description_stderr, // self one is different
        };

    return (OutStream){.data = stderr, .vtable = &STDERR_VTABLE};
}
