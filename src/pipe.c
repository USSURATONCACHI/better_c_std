#include <errno.h>
#include <string.h>
#include <better_c_std/pipe.h>
#include <better_c_std/string/str.h>
#include <better_c_std/prettify.h>

#ifdef _WIN32
BcstdPipe BcstdPipe_open() {
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    BcstdPipe pipe;

    if (!CreatePipe(&pipe.read, &pipe.write, &sa, 0)) {
        return (BcstdPipeResult) {
            .is_ok = false,
            .error = BcstdStr_literal("Failed to create pipe."),
        };
    } else {
        return (BcstdPipeResult) {
            .is_ok = true,
            .ok = pipe,
        };
    }
}
#else
BcstdPipeResult BcstdPipe_open() {
    BcstdPipeHandle handles[2];

    if (pipe(handles) == -1) {
        return (BcstdPipeResult) {
            .is_ok = false,
            .error = BcstdStr_literal(strerror(errno)),
        };
    } else {
        return (BcstdPipeResult) {
            .is_ok = true,
            .ok = (BcstdPipe) {
                .read = handles[0],
                .write = handles[1],
            },
        };
    }
}
#endif

void BcstdPipe_close(BcstdPipe pipe) {
    BcstdPipeHandle_close(pipe.read);
    BcstdPipeHandle_close(pipe.write);
}

void BcstdPipeHandle_close(BcstdPipeHandle handle) {
#ifdef _WIN32
    CloseHandle(handle);
#else
    close(handle);
#endif
}


static int    stream_putc              (BcstdPipeHandle* self, int);
static int    stream_puts              (BcstdPipeHandle* self, const char* str);
static int    stream_put_slice         (BcstdPipeHandle* self, const char* str, size_t length);
static size_t stream_get_available_size(BcstdPipeHandle* self);
static BcstdStr  stream_description       (BcstdPipeHandle* self);

static const OutStreamVtable* get_stream_vtable() {
    static const OutStreamVtable vtable = {
        .putc = (void*) stream_putc,
        .puts = (void*) stream_puts,
        .put_slice = (void*) stream_put_slice,
        .get_available_size = (void*) stream_get_available_size,
        .description = (void*) stream_description,
    };
    return &vtable;
}

OutStream BcstdPipe_outstream(BcstdPipeHandle* pipe) {
    return (OutStream) {
        .data = pipe,
        .vtable = get_stream_vtable(),
    };
}


static int stream_putc(BcstdPipeHandle* pipe, int data) {
    #ifdef _WIN32
        WriteFile(*pipe, &data, 1, NULL, NULL);
    #else
        write(*pipe, &data, 1);
    #endif
    return data;
}
static int stream_puts(BcstdPipeHandle* pipe, const char* str) {
    return stream_put_slice(pipe, str, strlen(str));
}
static int stream_put_slice(BcstdPipeHandle* pipe, const char* str, size_t length) {
    #ifdef _WIN32
        WriteFile(*pipe, str, length, NULL, NULL);
    #else
        write(*pipe, str, length);
    #endif
    return '\n';
}
static size_t stream_get_available_size(BcstdPipeHandle* pipe) {
    unused(pipe);
    return SIZE_MAX;
}
static BcstdStr stream_description(BcstdPipeHandle* pipe) {
    unused(pipe);
    #ifdef _WIN32
        return BcstdStr_literal("better_c_std Win32 pipe");
    #else
        return BcstdStr_literal("better_c_std Unix pipe");
    #endif
}