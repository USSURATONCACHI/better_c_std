#include <better_c_std/io/printers/integers.h>

// typedef struct {

//     void (*free)(void* self_data);
// } BcstdPrinterVtable;

// struct BcstdPrinter {
//     BcstdStr name;
//     void* data;
//     BcstdPrinterVtable* vtable;
// };

static bool fn_is_supported(const BcstdPrinter* self, const vec_BcstdPrinter* available_printers, StrSlice format_params, const BcstdPrintArg* arg) {
    unused(self);
    unused(available_printers);
    unused(format_params);
    
    const char* typename = arg->type_name;

    const char* supported_ints[] = {
        "int8_t", "uint8_t",
        "int16_t", "uint16_t",
        "int32_t", "uint32_t",
        "int64_t", "uint64_t",
        "intmax_t", "uintmax_t",
        "intptr_t", "uintptr_t",

        "short", 
        "int",
        "int",
    }; 

    if (strcmp(typename, "int8_t")) {}
}

static void fn_print(BcstdPrinter* self, const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format_params, BcstdVaList* args, BcstdPrintArg arg) {
    unused(self);
}

BcstdPrinter BcstdPrinter_integers_create() {
    static const BcstdPrinterVtable VTABLE = {
        .free = NULL,
        .is_supported = fn_is_supported,
        .print = fn_print,
    };

    return (BcstdPrinter) {
        .data = NULL,
        .name = BcstdStr_literal("integers"),
        .vtable = &VTABLE,
    };
}