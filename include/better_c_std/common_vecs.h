#ifndef BETTER_C_STD_COMMON_VECS_H_
#define BETTER_C_STD_COMMON_VECS_H_

// vec_char header + implementation
#define VECTOR_ITEM_TYPE char
#include "vector.h"

// vec_void_ptr header + implementation
typedef void* void_ptr;
#define VECTOR_ITEM_TYPE void_ptr
#include "vector.h"

#define VECTOR_ITEM_TYPE double
#include "vector.h"

#endif  // BETTER_C_STD_COMMON_VECS_H_