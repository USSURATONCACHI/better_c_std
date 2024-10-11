#include <better_c_std/string/str_slice.h>

#include <stdbool.h>
#include <string.h>

#include <better_c_std/prettify.h>

bool BcstdStrSlice_eq_ccp(StrSlice slice, const char* string) {
    return slice.length == (size_t)strlen(string) &&
            strncmp(slice.start, string, slice.length) == 0;
}

bool BcstdStrSlice_eq(StrSlice a, StrSlice b) {
    return a.length == b.length && strncmp(a.start, b.start, a.length) == 0;
}

BcstdStr BcstdStrSlice_to_owned(StrSlice slice) {
    char* buffer = (char*)malloc((slice.length + 1) * sizeof(char));
    strncpy(buffer, slice.start, slice.length + 1);
    buffer[slice.length] = '\0';
    return (BcstdStr){.is_owned = true, .string = buffer};
}

StrSlice BcstdStrSlice_from_BcstdStr(const BcstdStr* str) {
    return (StrSlice){.start = str->string, .length = strlen(str->string)};
}

StrSlice BcstdStrSlice_from_string(const char* str) {
    return (StrSlice){.start = str, .length = strlen(str)};
}