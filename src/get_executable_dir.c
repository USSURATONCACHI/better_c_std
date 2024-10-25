#include <better_c_std/get_executable_dir.h>
#include <better_c_std/string.h>

#include <limits.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef __linux__
#include <unistd.h>

BcstdStr Bcstd_get_executable_dir() {
    static char exe_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    
    if (len != -1) {
        exe_path[len] = '\0';  // Null-terminate the path
        return (BcstdStr) {
            .is_owned = true,
            .string = strdup(dirname(exe_path)),
        };  // Extract the directory from the path
    }

    return BcstdStr_literal(NULL);  // Could not determine executable directory
}

#elif _WIN32
#include <windows.h>

BcstdStr Bcstd_get_executable_dir() {
    static char exe_path[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    
    if (len > 0 && len < MAX_PATH) {
        return (BcstdStr) {
            .is_owned = true,
            .string = strdup(dirname(exe_path)),
        };  // Extract the directory from the path
    }

    return BcstdStr_literal(NULL);  // Could not determine executable directory
}

#elif __APPLE__
#include <mach-o/dyld.h>

BcstdStr Bcstd_get_executable_dir() {
    static char exe_path[PATH_MAX];
    uint32_t size = sizeof(exe_path);
    
    if (_NSGetExecutablePath(exe_path, &size) == 0) {
        return (BcstdStr) {
            .is_owned = true,
            .string = strdup(dirname(exe_path)),
        };  // Extract the directory from the path
    }

    return BcstdStr_literal(NULL);  // Could not determine executable directory
}

#else
#error "Unsupported platform"
#endif
