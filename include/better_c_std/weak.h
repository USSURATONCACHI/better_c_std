#ifndef BETTER_C_STD_WEAK_H_
#define BETTER_C_STD_WEAK_H_

#ifdef __GNUC__
    // GCC or Clang: both use the same syntax
    #define WEAK __attribute__((weak))
#elif defined(_MSC_VER)
    #define WEAK __declspec(selectany)
#else
    #error Unknown syntax for weak symbols
    #define WEAK
#endif

#endif // BETTER_C_STD_WEAK_H_