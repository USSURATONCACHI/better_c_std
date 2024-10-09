#include <better_c_std/allocator.h>

#include <stdlib.h>

#include <better_c_std/prettify.h>
#include <better_c_std/mem_region.h> 

static vec_MemRegion regions = {.data = NULL, .capacity = 0, .length = 0};
static bool is_enabled = false;
static bool print_debug = false;

void* BcstdDebugAllocator_malloc(size_t size) {
    if (!is_enabled)
        return malloc(size);

    MemRegion r = {
            .ptr = malloc(size),
            .size = size,
    };
    vec_MemRegion_push(&regions, r);

    if (print_debug)
        debugln("Alloc for %ld at %p", (long)r.size, r.ptr);

    return r.ptr;
}

void* BcstdDebugAllocator_realloc(void* mem, size_t size) {
    if (!is_enabled)
        return realloc(mem, size);

    if (mem) {
        for (size_t i = 0; i < regions.length; i++) {
            if (regions.data[i].ptr == mem) {
                void* new_mem = realloc(mem, size);
                // debugln("Realloc %p with %ld bytes into %p with %ld bytes",
                // regions.data[i].ptr, (long)regions.data[i].size, new_mem,
                // (long)size);
                regions.data[i].ptr = new_mem;
                regions.data[i].size = size;
                return new_mem;
            }
        }

        panic("Unknown realloc pointer: %p", mem);
    } else {
        return BcstdDebugAllocator_malloc(size);
    }
}
void BcstdDebugAllocator_free(void* mem) {
    if (!is_enabled)
        free(mem);

    if (!mem) return;

    for (size_t i = 0; i < regions.length; i++) {
        if (regions.data[i].ptr == mem) {
            // debugln("Freeing %p", mem);
            free(mem);
            vec_MemRegion_delete_fast(&regions, i);
            return;
        }
    }

    debugln("Unknown free pointer: %p", mem);
    // free(mem);
}

void BcstdDebugAllocator_disable() {
    is_enabled = false;
}
void BcstdDebugAllocator_enable() {
    is_enabled = true;
}


void BcstdDebugAllocator_free_all() {
    for (size_t i = 0; i < regions.length; i++) {
        debugln("Non freed allocation at %p for %d bytes. Freeing forcibly...", regions.data[i].ptr, (int)regions.data[i].size);
        free(regions.data[i].ptr);
    }
    vec_MemRegion_free(regions);
    regions = vec_MemRegion_create();
}


void BcstdDebugAllocator_dump() {
    debugln("Allocator has %d active memory regions (cap %d):", regions.length,
                    regions.capacity);

    for (size_t i = 0; i < regions.length; i++) {
        debugc("Ptr: %p | Size: %d\n", regions.data[i].ptr,
                        (int)regions.data[i].size);
    }

    debugln("Allocator dump done");
}

void BcstdDebugAllocator_dump_short() {
    size_t sum_size = 0;

    for (size_t i = 0; i < regions.length; i++) sum_size += regions.data[i].size;

    debugln("Allocator has %d(/%d) active memory regions for the total size of %ld", regions.length, regions.capacity, (long)sum_size);
}