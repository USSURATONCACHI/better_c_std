#include <better_c_std/io/buffer_out_stream.h>
#include <better_c_std/prettify.h>


static int putc_buf(BufferOutStream* self, int c) {
    if (self->pos < self->length) {
        self->buffer[self->pos++] = c;
        return c;
    } else {
        return EOF;
    }
}
static int puts_buf(BufferOutStream* self, const char* str) {
    if (self->pos >= self->length) return EOF;

    size_t i;
    for (i = 0; str[i] != '\0' && (self->pos + i + 1) < self->length; i++)
        self->buffer[i + self->pos] = str[i];
    self->buffer[i + self->pos] = '\0';
    self->pos += i + 1;

    if (str[i] == '\0') {
        return '\n';
    } else {
        return EOF;
    }
}

static int put_slice_buf(BufferOutStream* self, const char* str, size_t length) {
    if (self->pos >= self->length) return EOF;

    size_t i;
    for (i = 0; i < length && (self->pos + i + 1) < self->length; i++)
        self->buffer[i + self->pos] = str[i];
    self->buffer[i + self->pos] = '\0';
    self->pos += i + 1;

    if (str[i] == '\0') {
        return '\n';
    } else {
        return EOF;
    }
}

static int write_buf(BufferOutStream* self, const void* data, size_t length) {
    if (self->pos >= self->length) 
        return EOF;

    size_t i;
    for (i = 0; i < length && (self->pos + i + 1) < self->length; i++)
        self->buffer[i + self->pos] = ((const char*)data)[i];
    self->buffer[i + self->pos] = '\0';
    self->pos += length;
    return i;
}
static size_t get_size_buf(BufferOutStream* self) {
    return self->pos - self->length;
}
static BcstdStr description_buf(BufferOutStream* self) {
    unused(self);
    return BcstdStr_literal("BufferOutStream(?)");
}

OutStream outstream_from_buffer(BufferOutStream* b) {
    static const OutStreamVtable BUFFER_VTABLE = {
            .putc               = (void*)putc_buf,
            .puts               = (void*)puts_buf,
            .put_slice          = (void*)put_slice_buf,
            .get_available_size = (void*)get_size_buf,
            .description        = (void*)description_buf,
            .write              = (void*)write_buf,
        };

    return (OutStream){.data = b, .vtable = &BUFFER_VTABLE};
}