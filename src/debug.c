
#include <better_c_std/string.h>
#include <better_c_std/io.h>
#include <better_c_std/prettify.h>

static int tabs = 0;
static bool is_stream_set = false;
static OutStream stream = { NULL, NULL };

void debug_push() {
    tabs++;
}
void debug_pop() {
    tabs--;
}
void debug_print_tabs() {
    for (int i = 0; i < tabs; i++) 
        x_sprintf(debug_get_out_stream(), "|   ");
}


OutStream debug_get_default_outstream() {
    return outstream_stdout();
}
OutStream debug_get_out_stream() {
    if (!is_stream_set) {
        stream = debug_get_default_outstream();
    }
    return stream;
}
void debug_set_out_stream(OutStream os) {
    is_stream_set = true;
    stream = os;
}