#ifndef BETTER_C_STD_MEM_REGION_H_
#define BETTER_C_STD_MEM_REGION_H_

#include <stddef.h>

typedef struct MemRegion {
  void* ptr;
  size_t size;
} MemRegion;

// vec_MemRegion
#define VECTOR_ITEM_TYPE MemRegion
#include "vector.h"

#endif // BETTER_C_STD_MEM_REGION_H_