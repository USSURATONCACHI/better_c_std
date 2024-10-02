#include <string.h>

#include <better_c_std/io.h>
#include <better_c_std/string.h>
#include <better_c_std/prettify.h>

typedef void (*PrinterFn)(OutStream stream, VaListWrap* list,
                          int* total_written);

static void printer_slice(OutStream stream, VaListWrap* list,
                          int* total_written);

#define FORMATS { "$slice" }
#define PRINTERS { printer_slice }

int x_printf_ext_fmt_length(const char* format) {
  if (format[0] == '\0') return 0;

  format++;

  const char* const formats[] = FORMATS;
  for (int i = 0; i < (int)LEN(formats); i++) {
    int len = strlen(formats[i]);
    if (strncmp(format, formats[i], len) == 0) {
      return len + 1;
    }
  }

  return 0;
}
void x_printf_ext_print(OutStream stream, const char* format, VaListWrap* list,
                        int* total_written) {
  if (format[0] == '\0') return;
  format++;

  const char* const formats[] = FORMATS;
  const PrinterFn printers[] = PRINTERS;
  for (int i = 0; i < (int)LEN(formats); i++)
    if (strncmp(format, formats[i], strlen(formats[i])) == 0) {
      printers[i](stream, list, total_written);
      return;
    }

  return;
}

static void printer_slice(OutStream stream, VaListWrap* list,
                          int* total_written) {
  StrSlice slice = va_arg(list->list, StrSlice);
  outstream_put_slice(slice.start, slice.length, stream);
  (*total_written) += slice.length;
}

static void printer_printable(OutStream stream, VaListWrap* list,
                              int* total_written) {
  Printable val = va_arg(list->list, Printable);
  printable_print(val, stream);
  (*total_written) += 5;  // TODO
}