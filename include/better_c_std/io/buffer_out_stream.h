#ifndef BETTER_C_STD_IO_BUFFER_OUT_STREAM_H_
#define BETTER_C_STD_IO_BUFFER_OUT_STREAM_H_

#include <better_c_std/io/out_stream.h>

typedef struct BufferOutStream {
    char* buffer;
    size_t length;
    size_t pos;
} BufferOutStream;
OutStream outstream_from_buffer(BufferOutStream* b);

#endif // BETTER_C_STD_IO_BUFFER_OUT_STREAM_H_