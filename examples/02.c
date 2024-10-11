#include <better_c_std/io.h>
#include <better_c_std/string.h>
#include <better_c_std/prettify.h>

int main(int argc, char** argv) {
    unused(argv);

    BcstdStr s1 = BcstdStr_owned("Hello, world");
    BcstdStr s2 = BcstdStr_owned("Program has %d args", argc);
    BcstdStr s3 = BcstdStr_literal("This string does not introduce allocations");

    x_printf("%s\n", s1.string);
    x_printf("%s\n", s2.string);
    x_printf("%s\n", s3.string);

    BcstdStr_free(s1);
    BcstdStr_free(s2);
    BcstdStr_free(s3);
}