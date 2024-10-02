
#include <better_c_std/string.h>
#include <better_c_std/io.h>
#include <better_c_std/prettify.h>

static int tabs = 0;

void debug_push() {
  tabs++;
}
void debug_pop() {
  tabs--;
}
void debug_print_tabs() {
  for (int i = 0; i < tabs; i++) x_sprintf(DEBUG_OUT, "|   ");
}