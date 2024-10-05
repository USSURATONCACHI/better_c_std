cmake_minimum_required(VERSION 3.5)
project(s21_ussur_better_c_std VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BCSTD_BUILD_EXAMPLES "Build examples?" ON)

add_library(s21_ussur_better_c_std SHARED
    src/allocator.c
    src/common_vecs.c
    src/debug.c
    src/printf_extensions.c
    src/pipe.c

    src/string/str_slice.c
    src/string/str_t.c
    src/string/string_stream.c

    src/io/out_stream.c
    src/io/printable.c
    src/io/x_printf.c
)

target_include_directories(s21_ussur_better_c_std PUBLIC include)

include(GNUInstallDirs)
install(
    TARGETS s21_ussur_better_c_std
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install the header files
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


if (BCSTD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()