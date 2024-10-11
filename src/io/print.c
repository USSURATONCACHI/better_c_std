#include <better_c_std/io/print.h>
#include <better_c_std/io/print_arg.h>
#include <stdarg.h>
#include <pthread.h>

void bcstd_print(const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format, ...) {
    BcstdVaList list;
    va_start(list.list, format);
    bcstd_vprint(available_printers, os, format, &list);
    va_end(list.list);
}


void bcstd_print_ccp(const vec_BcstdPrinter* available_printers, OutStream os, const char* format, ...) {
    BcstdVaList list;
    va_start(list.list, format);
    bcstd_vprint_ccp(available_printers, os, format, &list);
    va_end(list.list);
}

void bcstd_vprint_ccp(const vec_BcstdPrinter* available_printers, OutStream os, const char* format, BcstdVaList* va_list) {
    bcstd_vprint(available_printers, os, BcstdStrSlice_from_string(format), va_list);
}

static StrSlice next_block_content(char open, char close, StrSlice string);
static void print_whatever_in_block(const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format_block, BcstdPrintArg arg, BcstdVaList* va_list); 

void bcstd_vprint(const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format, BcstdVaList* va_list) {
    static const vec_BcstdPrinter NO_PRINTERS = { NULL, 0, 0 };
    if (format.start == NULL)
        return;

    if (available_printers == NULL)
        available_printers = &NO_PRINTERS;

    bool no_more_args = false;
    while (format.length > 0) {
        StrSlice format_block = next_block_content('{', '}', format);
        StrSlice plain_text;
        StrSlice next_format;

        if (format_block.start == NULL) { // No format block
            plain_text = format;
            next_format = StrSlice_NULL;
        } else {
            size_t format_block_offset = format_block.start - format.start;
            plain_text = (StrSlice) {
                .start = format.start,
                .length = format_block_offset,
            };
            next_format = (StrSlice) {
                .start = format_block.start + format_block.length + 1,
                .length =  format.length - format_block_offset - format_block.length - 1,
            };
        }

        OutStream_put_slice(os, plain_text.start, plain_text.length);

        BcstdPrintArg arg = $end;
        if (!no_more_args)
            arg = va_arg(va_list->list, BcstdPrintArg);

        if (arg.printer_name == NULL)
            no_more_args = true;

        if (format_block.start) {
            if (arg.printer_name == NULL)
                OutStream_puts(os, "{ None }");
            else 
                print_whatever_in_block(available_printers, os, format_block, arg, va_list);
        }

        format = next_format;
    }
}

static StrSlice next_block_content(char open, char close, StrSlice string) {
    size_t depth = 0;
    size_t block_start = 0;

    bool is_escaped = false;
    for (size_t i = 0; i < string.length; i++) {
        char c = string.start[i];

        if (!is_escaped && c == '\\') {
            is_escaped = true;
            continue;
        } else {
            is_escaped = false;
        }

        if (!is_escaped) {
            if (c == open) {
                if (depth == 0)
                    block_start = i;

                depth++;
            } else if (c == close) {
                depth--;

                if (depth == 0) {
                    return (StrSlice) { 
                        .start = string.start + block_start,
                        .length = i - block_start,
                    };
                }
            }
        }
    }

    return StrSlice_NULL;
}

static void print_whatever_in_block(const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format_block, BcstdPrintArg arg, BcstdVaList* va_list) {
    for (size_t i = available_printers->length; i > 0; i--) {
        BcstdPrinter* printer = &available_printers->data[i - 1]; 
        // if (strcmp(arg.printer_name, printer->name.string) != 0)
        //     continue;

        if (!BcstdPrinter_is_supported(printer, available_printers, format_block, &arg))
            continue;

        BcstdPrinter_print(printer, available_printers, os, format_block, va_list, arg);
        return;
    }

    OutStream_puts(os, "{");
    OutStream_puts(os, arg.type_name);
    OutStream_puts(os, "?}");
}

static pthread_mutex_t PrintDefMutex;
static vec_BcstdPrinter PrintDefPrinters = { NULL, 0, 0 };
static bool PrintDefIsInit = false;

static void bcstd_print_def_init();
void bcstd_print_def_lock() {
    pthread_mutex_lock(&PrintDefMutex);
    if (!PrintDefIsInit)
        bcstd_print_def_init();
}
void bcstd_print_def_unlock() {
    pthread_mutex_unlock(&PrintDefMutex);
}

void bcstd_print_def_add_printer(BcstdPrinter new_printer) {
    bcstd_print_def_lock();
    vec_BcstdPrinter_push(&PrintDefPrinters, new_printer);
    bcstd_print_def_unlock();
}

static void bcstd_print_def_init() {
    

    PrintDefIsInit = true;
}

const vec_BcstdPrinter* bcstd_print_def_get_printers() {
    return &PrintDefPrinters;
}