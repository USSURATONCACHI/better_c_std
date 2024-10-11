#ifndef BETTER_C_STD_IO_PRINTER_H_
#define BETTER_C_STD_IO_PRINTER_H_

#include <better_c_std/string/str_struct.h>
#include <better_c_std/string/str_slice.h>
#include <better_c_std/io/out_stream.h>
#include <better_c_std/io/va_list.h>
#include <better_c_std/io/print_arg.h>

typedef struct BcstdPrinter BcstdPrinter;

// struct vec_BcstdPrinter
#define VECTOR_ITEM_TYPE BcstdPrinter
#include <better_c_std/vector.h>

typedef struct {
    bool (*is_supported)(const BcstdPrinter* self, const vec_BcstdPrinter* available_printers, StrSlice format_params, const BcstdPrintArg* arg);
    void (*print)(BcstdPrinter* self, const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format_params, BcstdVaList* args, BcstdPrintArg arg);

    void (*free)(void* self_data);
} BcstdPrinterVtable;

struct BcstdPrinter {
    BcstdStr name;
    void* data;
    const BcstdPrinterVtable* vtable;
};

// Simple call wrappers
static inline bool BcstdPrinter_is_supported(const BcstdPrinter* self, const vec_BcstdPrinter* available_printers, StrSlice format_params, const BcstdPrintArg* arg) {
    if (self->vtable->is_supported)
        return self->vtable->is_supported(self, available_printers, format_params, arg);
    return false;
}

static inline void BcstdPrinter_print(BcstdPrinter* self, const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format_params, BcstdVaList* args, BcstdPrintArg arg) {
    if (self->vtable->print)
        self->vtable->print(self, available_printers, os, format_params, args, arg);
}

static inline void BcstdPrinter_free(BcstdPrinter self) {
    BcstdStr_free(self.name);
    if (self.vtable->free)
        self.vtable->free(self.data);
}


#endif // BETTER_C_STD_IO_PRINTER_H_