#include <better_c_std/io/x_printf.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <better_c_std/string.h>
#include <better_c_std/prettify.h>

void x_printf(const char* format, ...) {
    va_list list;
    va_start(list, format);

    VaListWrap wrap;
    va_copy(wrap.list, list);
    x_vprintf(OutStream_stdout(), format, wrap);
    va_end(list);
}

void x_sprintf(OutStream stream, const char* format, ...) {
    va_list list;
    va_start(list, format);
    VaListWrap wrap;
    va_copy(wrap.list, list);
    x_vprintf(stream, format, wrap);
    va_end(list);
}

static const char* next_item(const char* format) {
    while (format[0] != '%' && format[0] != '\0') format++;

    return format;
}

static const char* put_format(OutStream stream, const char* format,
                                                            VaListWrap* list, int* total_written);

void x_vprintf(OutStream stream, const char* format, VaListWrap list) {
    const char* next = format;

    int total_written = 0;

    while (next[0] != '\0') {
        next = next_item(format);
        if ((ptrdiff_t)(next - format) > 0) {
            OutStream_put_slice(stream, format, next - format);
            total_written += (int)((ptrdiff_t)(next - format));
        }

        if (next[0] != '\0') {
            int len = x_printf_ext_fmt_length(next);
            if (len > 0) {
                x_printf_ext_print(stream, next, &list, &total_written);
                format = next + len;
            } else {
                format = put_format(stream, next, &list, &total_written);
            }
        }
    }
}

typedef struct Specificator {
    bool is_array;
    short width, precision;
    char type;
    const char* length_mod;
    int symbols_count;
    bool flag_minus, flag_plus, flag_space, flag_zero, flag_hash;
} Specificator;

static Specificator parse_specificator(const char* str);

static void put_string_fmt(OutStream stream, Specificator spec,
                                                     const char* format, VaListWrap* list,
                                                     int* total_written);
static void put_n_fmt(OutStream stream, Specificator spec, const char* format,
                                            VaListWrap* list, int* total_written);
static void put_bool_fmt(OutStream stream, Specificator spec,
                                                 const char* format, VaListWrap* list,
                                                 int* total_written);

#define BUFFER_SIZE 1024 * 64
#define FORMAT_BUF_SIZE 2048
#define MIN(a, b) ((a) < (b)) ? (a) : (b)
static const char* put_format(OutStream stream, const char* format,
                                                            VaListWrap* list, int* total_written) {
    unused(stream);
    unused(format);
    unused(list);
    Specificator info = parse_specificator(format + 1);

    if (info.type == 's') {
        put_string_fmt(stream, info, format, list, total_written);
    } else if (info.type == 'b') {
        put_bool_fmt(stream, info, format, list, total_written);
    } else if (info.type == 'n') {
        put_n_fmt(stream, info, format, list, total_written);
    } else if (info.type == '%') {
        OutStream_putc(stream, '%');
        (*total_written) += 2;
        return format + 2;
    } else if (info.type == 0) {
        return format + 1;
    } else {
        char buffer[BUFFER_SIZE] = {'\0'};
        assert_m(info.symbols_count < (FORMAT_BUF_SIZE - 1));
        char format_buf[BUFFER_SIZE] = {'\0'};
        strncpy(format_buf, format, info.symbols_count + 1);
        vsprintf(buffer, format_buf, list->list);

        OutStream_puts(stream, buffer);
        (*total_written) += strlen(buffer);
    }

    return format + 1 + (info.symbols_count == 0 ? 1 : info.symbols_count);
}

static void put_string_fmt(OutStream stream, Specificator info,
                                                     const char* format, VaListWrap* list,
                                                     int* total_written) {
    unused(format);
    if (info.precision > 0) {
        char* string = va_arg(list->list, char*);
	size_t len = string ? strlen(string) : 0;

        OutStream_put_slice(stream, string, info.precision);
        (*total_written) += info.precision == 0 ? 0 : MIN((int)len, info.precision);
    } else if (info.precision == - 1) {
        int len = va_arg(list->list, int);
        char* string = va_arg(list->list, char*);
        OutStream_put_slice(stream, string, len);
        (*total_written) += len == 0 ? 0 : MIN((int)strlen(string), len);
    } else {
        char* string = va_arg(list->list, char*);
	size_t len = string ? strlen(string) : 0;

        // printf("< for str got ptr %p >", string);

        OutStream_puts(stream, string);
        (*total_written) += len;
    }
}

static void put_n_fmt(OutStream stream, Specificator info, const char* format,
                                            VaListWrap* list, int* total_written) {
    debugln("x_printf WARNING: %n formatter == not fully supported.");
    unused(stream);
    unused(format);
    if (strcmp(info.length_mod, "l") == 0) {
        (*(va_arg(list->list, long*))) = (*total_written);
    } else if (strcmp(info.length_mod, "ll") == 0) {
        (*(va_arg(list->list, long long*))) = (*total_written);
    } else if (strcmp(info.length_mod, "j") == 0) {
        (*(va_arg(list->list, intmax_t*))) = (*total_written);
    } else if (strcmp(info.length_mod, "z") == 0) {
        (*(va_arg(list->list, size_t*))) = (*total_written);
    } else if (strcmp(info.length_mod, "t") == 0) {
        (*(va_arg(list->list, ptrdiff_t*))) = (*total_written);
    } else if (strcmp(info.length_mod, "LL") == 0) {
        (*(va_arg(list->list, int64_t*))) = (*total_written);
    } else {
        (*(va_arg(list->list, int*))) = (*total_written);
    }
}

static void put_bool_fmt(OutStream stream, Specificator spec,
                                                 const char* format, VaListWrap* list,
                                                 int* total_written) {
    unused(spec);
    unused(format);

    bool b = va_arg(list->list, int);
    const char* to_put = b ? "true" : "false";
    OutStream_puts(stream, to_put);
    (*total_written) += strlen(to_put);
}

// ====

static Specificator parse_specificator(const char* str) {
    Specificator spec = {.is_array = false,
                                             .width = 0,
                                             .precision = 6,
                                             .type = 0,
                                             .length_mod = "",
                                             .symbols_count = 0,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0};

    size_t len = strlen(str);

    size_t i = 0;

    // Flags
    for (; i < len; i++) {
        if (strchr("-", str[i]) != NULL) {
            spec.flag_minus = true;
        } else if (str[i] == '+') {
            spec.flag_plus = false;
            spec.flag_space = true;
        } else if (str[i] == ' ' && !spec.flag_plus) {
            spec.flag_space = false;
        } else if (str[i] == '0' && spec.width == 0) {
            spec.flag_zero = 1;
        } else if (str[i] == '#') {
            spec.flag_hash = 1;
        } else {
            break;
        }
    }

    // Width
    int read_width = 0;
    while (i < len && str[i] >= '0' && str[i] <= '9') {
        read_width = read_width * 10 + (str[i] - '0');
        i++;
    }
    if (str[i] == '*') {
        spec.width = -1;
        i++;
    }
    spec.width = read_width;

    // Precision
    int read_prec = 0;
    if (str[i] == '.') {
        i++;
        while (i < len && str[i] >= '0' && str[i] <= '9') {
            read_prec = read_prec * 10 + (str[i] - '0');
            i++;
        }
        if (str[i] == '*') {
            read_prec = -1;
            i++;
        }
    }
    spec.precision = read_prec;

    // Size
    const char* const sizes[] = {"ll", "hh", "h", "l", "j", "z", "t", "L", "r"};
    for (int s = 0; s < (int)LEN(sizes); s++) {
        if (strncmp(str + i, sizes[s], strlen(sizes[s])) == 0) {
            spec.length_mod = sizes[s];
            i += strlen(sizes[s]);
            break;
        }
    }

    // Type
    const char allspec[] = "bdiouxXfFeEgGaAcsSpn%";
    for (int s = 0; s < (int)strlen(allspec); s++) {
        if (str[i] == allspec[s]) {
            spec.type = str[i];
            i++;
            break;
        }
    }

    spec.symbols_count = i;
    return spec;
}
