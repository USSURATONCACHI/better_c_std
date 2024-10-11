#include <better_c_std/string.h>
#include <better_c_std/prettify.h>
#include <better_c_std/io/print.h>

int main() {
    int a = 4;
    int b = 6;
    int c = a + b;

    println("Hello, world! {} + {} = {}\n", $(int, a), $(int, b), $(int, c));
}