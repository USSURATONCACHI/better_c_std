#ifndef BETTER_C_STD_PIPE_H_
#define BETTER_C_STD_PIPE_H_

#include <stdbool.h>
#include <better_c_std/io/out_stream.h>
#include <better_c_std/result.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
#endif

// Cross-platform Pipe structure
#ifdef _WIN32
    typedef HANDLE BcstdPipeHandle;
#else
    typedef int BcstdPipeHandle;
#endif

typedef struct {
    BcstdPipeHandle read;
    BcstdPipeHandle write;
} BcstdPipe;

typedef STRUCT_RESULT(BcstdPipe, BcstdStr) BcstdPipeResult;

BcstdPipeResult BcstdPipe_open();
void BcstdPipe_close(BcstdPipe pipe);
void BcstdPipeHandle_close(BcstdPipeHandle handle);

#endif // BETTER_C_STD_PIPE_H_