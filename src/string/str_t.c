#include <better_c_std/string/str_t.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <better_c_std/prettify.h>
#include <better_c_std/string/string_stream.h>

str_t str_literal(const char* literal) { return (str_t){literal, false}; }
str_t str_owned(const char* format, ...) {
    va_list list;
    va_start(list, format);

    StringStream builder = string_stream_create();
    OutStream os = string_stream_stream(&builder);

    VaListWrap wrap;
    va_copy(wrap.list, list);
    x_vprintf(os, format, wrap);
    str_t result = string_stream_to_str_t(builder);

    va_end(list);

    return result;
}

str_t str_borrow(const str_t* from) {
    return (str_t){.is_owned = false, .string = from->string};
}

str_t str_raw_owned(char* text) {
    return (str_t){.is_owned = true, .string = text};
}

void str_free(str_t s) {
    if (s.is_owned) {
        free((void*)s.string);
    }
}

void str_free_p(str_t* s) {
    if (s == NULL) return;

    if (s->is_owned && s->string != NULL) free((void*)s->string);

    free(s);
}

str_t str_clone(const str_t* source) {
    if (source->is_owned) {
        int len = strlen(source->string) + 1;
        char* data = (char*) malloc(len * sizeof(char));
        assert_alloc(data);
        strcpy(data, source->string);
        return (str_t){
            .is_owned = source->is_owned,
            .string = data,
        };
    } else {
        return *source;
    }
}

void str_result_free(StrResult this) { 
    if (this.is_ok)
        str_free(this.ok);
    else
        str_free(this.error);
}

str_t read_file_to_str(const char* filename) {
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

    return (str_t){.is_owned = true, .string = buffer};
}

#include <better_c_std/string/vec_str_t.h>

#define VECTOR_C str_t
#define VECTOR_ITEM_DESTRUCTOR str_free
#define VECTOR_ITEM_CLONE str_clone
#include <better_c_std/vector.h>