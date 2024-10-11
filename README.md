# better_c_std

This library is a slowly growing collection of no-dependency utils to C language, that I write myself. It is fully built on C stdlib. It introduces 
+ streams, 
+ typed zero-overhead vectors (with destructors), 
+ a bit better string types and utils, 
+ custom debug allocator, 
+ printf extensions.

## Main points

### debug

```C
#include <better_c_std/prettify.h>

int main(int argc, char** argv) {
    debugln("Args count: %d, program name: %s", argc, argv[0]);

    debug("Hello, ");
    debugc("world\n"); //debugc does not print log point

    return 0;
}
```

Output:
```
LOG (/home/ussur/Projects/maze/better_c_std/examples/01.c:4): Args count: 1, program name: ./build/examples/example01
LOG (/home/ussur/Projects/maze/better_c_std/examples/01.c:6): Hello, world
```

You can substitute the point to where to print logs with `DEBUG_OUT` macro of type `OutStream`.

### vectors (zero overhead, have destructors for inner elements)

Check out readme at https://github.com/USSURATONCACHI/c_vector.h for examples and usage.

### BcstdStr

Can be both an allocated string with buffer, or a literal/reference.

```C
#include <better_c_std/io.h>
#include <better_c_std/string.h>

int main(int argc, char** argv) {
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
```
Output:
```
Hello, world
Program has 1 args
This string does not introduce allocations
```