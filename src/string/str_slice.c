#include <better_c_std/string/str_slice.h>

#include <stdbool.h>
#include <string.h>

#include <better_c_std/prettify.h>

bool str_slice_eq_ccp(StrSlice slice, const char* string) {
    return slice.length == (int)strlen(string) &&
            strncmp(slice.start, string, slice.length) == 0;
}

bool str_slice_eq(StrSlice a, StrSlice b) {
    return a.length == b.length && strncmp(a.start, b.start, a.length) == 0;
}

str_t str_slice_to_owned(StrSlice slice) {
    char* buffer = (char*)malloc((slice.length + 1) * sizeof(char));
    strncpy(buffer, slice.start, slice.length + 1);
    buffer[slice.length] = '\0';
    return (str_t){.is_owned = true, .string = buffer};
}

StrSlice str_slice_from_str_t(const str_t* str) {
    return (StrSlice){.start = str->string, .length = strlen(str->string)};
}

StrSlice str_slice_from_string(const char* str) {
    return (StrSlice){.start = str, .length = strlen(str)};
}