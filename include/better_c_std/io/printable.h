#ifndef BETTER_C_STD_IO_PRINTABLE_H_
#define BETTER_C_STD_IO_PRINTABLE_H_

#include <better_c_std/io/out_stream.h>

typedef struct Printable {
  const void* data;
  void (*print)(const void* data, OutStream stream);
} Printable;

void printable_print(Printable self, OutStream out);
void printable_print_std(Printable self);

Printable printable_string(const char* string);
Printable printable_outstream(const OutStream* self);

#endif  // BETTER_C_STD_IO_PRINTABLE_H_