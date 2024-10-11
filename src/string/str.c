#include <better_c_std/string/str.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <better_c_std/prettify.h>
#include <better_c_std/string/string_stream.h>

BcstdStr BcstdStr_literal(const char* literal) { return (BcstdStr){literal, false}; }
BcstdStr BcstdStr_owned(const char* format, ...) {
    va_list list;
    va_start(list, format);

    BcstdStringStream builder = BcstdStringStream_create();
    OutStream os = BcstdStringStream_stream(&builder);

    VaListWrap wrap;
    va_copy(wrap.list, list);
    x_vprintf(os, format, wrap);
    BcstdStr result = BcstdStringStream_to_BcstdStr(builder);

    va_end(list);

    return result;
}

BcstdStr BcstdStr_borrow(const BcstdStr* from) {
    return (BcstdStr){.is_owned = false, .string = from->string};
}

BcstdStr BcstdStr_raw_owned(char* text) {
    return (BcstdStr){.is_owned = true, .string = text};
}

void BcstdStr_free(BcstdStr s) {
    if (s.is_owned) {
        free((void*)s.string);
    }
}

void BcstdStr_free_p(BcstdStr* s) {
    if (s == NULL) return;

    if (s->is_owned && s->string != NULL) free((void*)s->string);

    free(s);
}

BcstdStr BcstdStr_clone(const BcstdStr* source) {
    if (source->is_owned) {
        int len = strlen(source->string) + 1;
        char* data = (char*) malloc(len * sizeof(char));
        assert_alloc(data);
        strcpy(data, source->string);
        return (BcstdStr){
            .is_owned = source->is_owned,
            .string = data,
        };
    } else {
        return *source;
    }
}

void BcstdStr_result_free(BcstdStrResult this) { 
    if (this.is_ok)
        BcstdStr_free(this.ok);
    else
        BcstdStr_free(this.error);
}

BcstdStr read_file_to_str(const char* filename) {
    char* buffer = NULL;
    long length;
    FILE* f = fopen(filename, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (char*)malloc((length + 1) * sizeof(char));
        assert_alloc(buffer);
        if (buffer) fread(buffer, 1, length, f);

        fclose(f);
        buffer[length] = '\0';

        for (long i = 0; buffer[i] != '\0'; i++)
            if (buffer[i] == '\r') 
                buffer[i] = ' ';
    }

    return (BcstdStr){.is_owned = true, .string = buffer};
}

#include <better_c_std/string/vec_str.h>

#define VECTOR_C BcstdStr
#define VECTOR_ITEM_DESTRUCTOR BcstdStr_free
#define VECTOR_ITEM_CLONE BcstdStr_clone
#include <better_c_std/vector.h>