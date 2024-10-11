#ifndef BETTER_C_STD_IO_PRINT_H_
#define BETTER_C_STD_IO_PRINT_H_

#include <better_c_std/io/printer.h>
#include <better_c_std/string/str_slice.h>
#include <better_c_std/prettify.h>

void bcstd_print(const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format, ...);
void bcstd_vprint(const vec_BcstdPrinter* available_printers, OutStream os, StrSlice format, BcstdVaList* va_list);

void bcstd_print_ccp(const vec_BcstdPrinter* available_printers, OutStream os, const char* format, ...);
void bcstd_vprint_ccp(const vec_BcstdPrinter* available_printers, OutStream os, const char* format, BcstdVaList* va_list);

// Default printing
void bcstd_print_def_lock();
void bcstd_print_def_unlock();

void bcstd_print_def_add_printer(BcstdPrinter new_printer);

const vec_BcstdPrinter* bcstd_print_def_get_printers();

#define print_nolock(...) bcstd_print_ccp(bcstd_print_def_get_printers(), OutStream_stdout(), __VA_ARGS__, $end)

#define println_nolock(...) \
    {                                             \
        print_nolock(__VA_ARGS__);                \
        OutStream_puts(OutStream_stdout(), "\n"); \
    }                                             \
    //;

#define print(...) \
    {                              \
        bcstd_print_def_lock();    \
        print_nolock(__VA_ARGS__); \
        bcstd_print_def_unlock();  \
    }                              \
    //;

#define println(...) \
    {                                \
        bcstd_print_def_lock();      \
        println_nolock(__VA_ARGS__); \
        bcstd_print_def_unlock();    \
    }                                \
    //;


#define eprint_nolock(...) bcstd_print_ccp(bcstd_print_def_get_printers(), OutStream_stderr(), __VA_ARGS__, $end)

#define eprintln_nolock(...) \
    {                                             \
        eprint_nolock(__VA_ARGS__);               \
        OutStream_puts(OutStream_stderr(), "\n"); \
    }                                             \
    //;

#define eprint(...) \
    {                              \
        bcstd_print_def_lock();    \
        eprint_nolock(__VA_ARGS__);\
        bcstd_print_def_unlock();  \
    }                              \
    //;

#define eprintln(...) \
    {                                \
        bcstd_print_def_lock();      \
        eprintln_nolock(__VA_ARGS__);\
        bcstd_print_def_unlock();    \
    }                                \
    //;


#endif // BETTER_C_STD_IO_PRINT_H_