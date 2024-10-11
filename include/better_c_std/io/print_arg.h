#ifndef BETTER_C_STD_IO_PRINT_ARG_H_
#define BETTER_C_STD_IO_PRINT_ARG_H_

#include <stddef.h>

typedef struct {
    const char* printer_name;
    const char* type_name;
    size_t arg_size;
    void* arg_data;
} BcstdPrintArg;

#define $(type, arg) \
    (BcstdPrintArg) {                  \
        .printer_name = STR(type),     \
        .type_name = STR(type),        \
        .arg_size = sizeof(arg),       \
        .arg_data = (void*)&(arg),     \
    }                                  \
    // ;

#define $P(printer, type, arg) \
    (BcstdPrintArg) {                  \
        .printer_name = (printer),     \
        .type_name = STR(type),        \
        .arg_size = sizeof(arg),       \
        .arg_data = (void*)&(arg),     \
    }                                  \
    // ;

#define $vec(type, arg) $p("vec", type, arg)
#define $end (BcstdPrintArg) { NULL, NULL, 0, NULL }


#endif // BETTER_C_STD_IO_PRINT_ARG_H_