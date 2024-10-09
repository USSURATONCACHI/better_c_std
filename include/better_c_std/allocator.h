#ifndef BETTER_C_STD_ALLOCATOR_H_
#define BETTER_C_STD_ALLOCATOR_H_

#include <stdint.h>
#include <stdlib.h>

// Simple debug allocator

void* BcstdDebugAllocator_malloc(size_t size);
void* BcstdDebugAllocator_realloc(void* mem, size_t size);
void BcstdDebugAllocator_free(void* mem);

void BcstdDebugAllocator_disable(); // make allocator pass calls straight to malloc, free, realloc, without middleware
void BcstdDebugAllocator_enable();

void BcstdDebugAllocator_free_all();

void BcstdDebugAllocator_dump();
void BcstdDebugAllocator_dump_short();


#define MALLOC BcstdDebugAllocator_malloc
#define REALLOC BcstdDebugAllocator_realloc
#define FREE BcstdDebugAllocator_free

// All <better_c_std/vector.h> after this include will use this debug allocator
#undef VECTOR_MALLOC_FN
#undef VECTOR_REALLOC_FN
#undef VECTOR_FREE_FN
#define VECTOR_MALLOC_FN BcstdDebugAllocator_malloc
#define VECTOR_REALLOC_FN BcstdDebugAllocator_realloc
#define VECTOR_FREE_FN BcstdDebugAllocator_free

#endif  // BETTER_C_STD_ALLOCATOR_H_