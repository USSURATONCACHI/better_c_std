#include <better_c_std/string/string_stream.h>

#include <string.h>

#include <better_c_std/prettify.h>
#include <better_c_std/string/str.h>

#define BUFFER_EXTRA_CAP 128
#define CHAR_REALLOC_COEF 4 / 3

void BcstdStringStream_free(BcstdStringStream this) { free(this.buffer); }

BcstdStringStream BcstdStringStream_create() {
    return (BcstdStringStream){
            .buffer = NULL,
            .capacity = 0,
            .length = 0,
    };
}

BcstdStringStream BcstdStringStream_clone(const BcstdStringStream* source) {
    if (source->buffer) {
        size_t new_cap = source->length + BUFFER_EXTRA_CAP;
        char* buffer = (char*)malloc(new_cap * sizeof(char));
        assert_alloc(buffer);
        memcpy(buffer, source->buffer, source->length);
        return (BcstdStringStream){
                .buffer = buffer,
                .capacity = new_cap,
                .length = source->length,
            };
    } else {
        return (*source);  // No data in hold, can just copy by value
    }
}

void BcstdStringStream_print(const BcstdStringStream* self, OutStream out) {
    OutStream_puts(out, "BcstdStringStream(");
    OutStream_put_slice(out, self->buffer, self->length);
    OutStream_puts(out, ")");
}

static void ss_realloc(BcstdStringStream* this, size_t add_size) {
    size_t new_cap =
            this->capacity * CHAR_REALLOC_COEF + add_size + BUFFER_EXTRA_CAP;
    this->buffer = (char*)realloc(this->buffer, new_cap);
    assert_alloc(this->buffer);
    this->capacity = new_cap;
}

static int ss_putc(BcstdStringStream* this, int c) {
    if (this->length >= this->capacity) ss_realloc(this, 1);

    this->buffer[this->length++] = c;
    return c;
}

static int ss_puts(BcstdStringStream* this, const char* str) {
    size_t len = strlen(str);
    if ((this->length + len) > this->capacity) ss_realloc(this, len);

    memcpy(&this->buffer[this->length], str, sizeof(char) * len);
    this->length += len;
    return '\n';
}

static int ss_write(BcstdStringStream* this, const void* data, size_t size) {
    if (size == 0) return 0;
    if ((this->length + size) > this->capacity) ss_realloc(this, size);

    memcpy(&this->buffer[this->length], data, sizeof(char) * size);
    this->length += size;
    return size;
}

static int ss_put_slice(BcstdStringStream* this, const char* str, int length) {
    if (length == 0) return '\n';
    if ((this->length + length) > this->capacity) ss_realloc(this, length);

    memcpy(&this->buffer[this->length], str, sizeof(char) * length);
    this->length += length;
    return '\n';
}

static size_t ss_get_available_size(BcstdStringStream* this) {
    unused(this);
    return SIZE_MAX;
}
static BcstdStr ss_description(BcstdStringStream* this) {
    unused(this);
    return BcstdStr_literal("BcstdStringStream");
}

OutStream BcstdStringStream_stream(BcstdStringStream* self) {
    static const OutStreamVtable TABLE = {
            .putc               = (void*)ss_putc,
            .puts               = (void*)ss_puts,
            .put_slice          = (void*)ss_put_slice,
            .description        = (void*)ss_description,
            .get_available_size = (void*)ss_get_available_size,
            .write              = (void*)ss_write,
        };
    return (OutStream){.data = self, .vtable = &TABLE};
}

char* BcstdStringStream_collect(BcstdStringStream self) {
    ss_putc(&self, '\0');
    return self.buffer;
}

BcstdStr BcstdStringStream_to_BcstdStr(BcstdStringStream self) {
    return BcstdStr_raw_owned(BcstdStringStream_collect(self));
}