#ifndef BETTER_C_STD_PRETTIFY_DEBUG_H_
#define BETTER_C_STD_PRETTIFY_DEBUG_H_

#include <better_c_std/io/x_printf.h>
#include <better_c_std/io/out_stream.h>

#ifndef DEBUG_OUT
    #define DEBUG_OUT outstream_stdout()
#endif

void debug_push();
void debug_pop();
void debug_print_tabs();

#define debugln(...)                                           \
  {                                                            \
    debug_print_tabs();                                        \
    x_sprintf(DEBUG_OUT, "LOG (%s:%d): ", __FILE__, __LINE__); \
    x_sprintf(DEBUG_OUT, __VA_ARGS__);                         \
    x_sprintf(DEBUG_OUT, "\n");                                \
  }
#define debug(...)                                             \
  {                                                            \
    debug_print_tabs();                                        \
    x_sprintf(DEBUG_OUT, "LOG (%s:%d): ", __FILE__, __LINE__); \
    x_sprintf(DEBUG_OUT, __VA_ARGS__);                         \
  }
#define debugc(...) \
  { x_sprintf(DEBUG_OUT, __VA_ARGS__); }

#endif  // BETTER_C_STD_PRETTIFY_DEBUG_H_