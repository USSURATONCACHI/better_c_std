#include <better_c_std/prettify.h>

int main(int argc, char** argv) {
    debugln("Args count: %d, program name: %s", argc, argv[0]);

    debug("Hello, ");
    debugc("world\n");

    return 0;
}