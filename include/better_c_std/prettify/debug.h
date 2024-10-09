#ifndef BETTER_C_STD_PRETTIFY_DEBUG_H_
#define BETTER_C_STD_PRETTIFY_DEBUG_H_

#include <better_c_std/io/x_printf.h>
#include <better_c_std/io/out_stream.h>


void debug_push();
void debug_pop();
void debug_print_tabs();

OutStream debug_get_default_outstream();
OutStream debug_get_outstream();
void debug_set_outstream(OutStream os);

#define debugln(...)                                                           \
    {                                                                          \
        debug_print_tabs();                                                    \
        x_sprintf(debug_get_outstream(), "LOG (%s:%d): ", __FILE__, __LINE__); \
        x_sprintf(debug_get_outstream(), __VA_ARGS__);                         \
        x_sprintf(debug_get_outstream(), "\n");                                \
    }
#define debug(...)                                                             \
    {                                                                          \
        debug_print_tabs();                                                    \
        x_sprintf(debug_get_outstream(), "LOG (%s:%d): ", __FILE__, __LINE__); \
        x_sprintf(debug_get_outstream(), __VA_ARGS__);                         \
    }
#define debugc(...) { x_sprintf(debug_get_outstream(), __VA_ARGS__); }

#endif  // BETTER_C_STD_PRETTIFY_DEBUG_H_