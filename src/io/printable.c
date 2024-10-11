#include <better_c_std/io/printable.h>
#include <better_c_std/string/str.h>
#include <better_c_std/prettify.h>

void printable_print(Printable this, OutStream out) {
    this.print(this.data, out);
}

void printable_print_std(Printable this) {
    this.print(this.data, OutStream_stdout());
}

Printable printable_string(const char* string) {
    return (Printable){.data = string, .print = (void*)OutStream_puts};
}

static void print_outstream(const OutStream* this, OutStream out) {
    OutStream_puts(out, "OutStream(");
    BcstdStr descr = OutStream_description(*this);
    OutStream_puts(out, descr.string);
    BcstdStr_free(descr);
    OutStream_puts(out, ")");
}

Printable printable_outstream(const OutStream* this) {
    return (Printable){.data = this, .print = (void*)print_outstream};
}