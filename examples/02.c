#include <better_c_std/io.h>
#include <better_c_std/string.h>

int main(int argc, char** argv) {
    str_t s1 = str_owned("Hello, world");
    str_t s2 = str_owned("Program has %d args", argc);
    str_t s3 = str_literal("This string does not introduce allocations");

    x_printf("%s\n", s1.string);
    x_printf("%s\n", s2.string);
    x_printf("%s\n", s3.string);

    str_free(s1);
    str_free(s2);
    str_free(s3);
}