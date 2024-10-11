/*

 /$$    /$$                      /$$                             /$$      
| $$   | $$                     | $$                            | $$      
| $$   | $$ /$$$$$$   /$$$$$$$ /$$$$$$    /$$$$$$   /$$$$$$     | $$$$$$$ 
|  $$ / $$//$$__  $$ /$$_____/|_  $$_/   /$$__  $$ /$$__  $$    | $$__  $$
 \  $$ $$/| $$$$$$$$| $$        | $$    | $$  \ $$| $$  \__/    | $$  \ $$
  \  $$$/ | $$_____/| $$        | $$ /$$| $$  | $$| $$          | $$  | $$
   \  $/  |  $$$$$$$|  $$$$$$$  |  $$$$/|  $$$$$$/| $$       /$$| $$  | $$
    \_/    \_______/ \_______/   \___/   \______/ |__/      |__/|__/  |__/
                                                                                
This header-only utility allows to create "templated" vectors in pure C.
You can also set custom destructors, copy constructors, and allocators.		

Item type is defined by macro `VECTOR_ITEM_TYPE`. 
It must be a single token without spaces or star symbols.
For complex type names use typedefs: `typedef char* char_ptr;`.

=============================
=   Simplest usage example  = 
=============================
```C
	#include <stdio.h>

	#define VECTOR_ITEM_TYPE int
	#define VECTOR_IMPLEMENTATION
	#include <better_c_std/vector.h>

	int main() {
		vec_int my_ints = vec_int_create();

		vec_int_push(&my_ints, 1);
		vec_int_push(&my_ints, 2);

		printf("%d\n", vec_int_popget(&my_ints)); // Output: 2
		printf("%d\n", vec_int_popget(&my_ints)); // Output: 1

		vec_int_free(my_ints); // pass by value to free
	}
```
=============================
=  All available functions  = (replace T with your type)
=============================
```
	// Create & free
	vec_T vec_T_create();         // Empty vector (this one does no allocations)
	void  vec_T_free(vec_T v);    // If `VECTOR_ITEM_DESTRUCTOR` is specified - calls destructor for each item first. Then, frees the data buffer.

	// Copying. 
		// All of these function call `VECTOR_ITEM_CLONE_FN` to copy each element if it is defined.
		// If copy constructor is not defined - they just `memcpy`.
	vec_T vec_T_clone      (const vec_T* source);   
	vec_T vec_T_create_copy(const T* source, size_t length);

	// Other constructors
	vec_T vec_T_from_raw   (T* source, size_t length); // Takes ownership of `T* source` and makes it it's own buffer
	vec_T vec_T_with_capacity    (size_t cap);         // `panic`s and terminates the process if allocation fails.
	vec_T vec_T_with_capacity_try(size_t cap);         // Like "_with_capacity", but will return vector with zero capacity, if allocation fails

	// General methods
	// `vec_T_popfree`, `vec_T_delete_fast`, `vec_T_delete_order` use destructor (`VECTOR_ITEM_DESTRUCTOR`) if provided.
	void  vec_T_push         (vec_T* vec, T item);
	void  vec_T_insert       (vec_T* vec, T item, size_t index);
	T     vec_T_popget       (vec_T* vec);  // Extracts and returns the last element
	void  vec_T_popfree      (vec_T* vec);  // Extracts and deletes the last element

	T     vec_T_at           (vec_T* vec, size_t i); // Returns element by value, without modifying it
	T*    vec_T_atref        (vec_T* vec, size_t i); // Returns pointer to an element 
													// (pointer is invalidated after `_push` and `_insert`, possibly after `_extract_` and `_delete_` methods)

	T     vec_T_extract_fast (vec_T* vec, size_t i); // O(1) - Extracts element and swaps with the last one
	T     vec_T_extract_order(vec_T* vec, size_t i); // O(n) - Extracts element and preserves order of elements
	void  vec_T_delete_fast  (vec_T* vec, size_t i); // O(1) - Deletes element and swaps with the last one
	void  vec_T_delete_order (vec_T* vec, size_t i); // O(n) - Deletes element and preserves order of elements
```

======================
=  All input macros  =
======================

	VECTOR_ITEM_TYPE <item_type> - specify item type to generate				  - default is undefined
	VECTOR_IMPLEMENTATION        - flag to generate implementations for functions - default is undefined
	VECTOR_NO_HEADERS            - flag to NOT generate headers                   - default is undefined
	VECTOR_MAKE_STATIC           - flag to prefix all functions with `static`     - default is undefined

	DEBUG                 - flag to `panic` instead of using `unreachable` - default is undefined
	VECTOR_API [anything] - prepend all functions with whatever - default is empty
	VECTOR_C <item_type>  - define only impls (no headers) for given vector - default is undefined

	== Specify how to handle memory and your type with these macros:
	VECTOR_MALLOC_FN       <void* fn(size_t)>         	(default is `malloc`)
	VECTOR_FREE_FN         <void  fn(void*)>          	(default is `free`)
	VECTOR_REALLOC_FN      <void* fn(void*, size_t)>  	(default is undefined)
	VECTOR_ITEM_CLONE_FN   <T     fn(T)>              	(default is undefined)
	VECTOR_ITEM_DESTRUCTOR <void  fn(T)>              	(default is undefined)


 */
 

// GCC does not fully understand what we are doing, so we heed to set this
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#ifndef VECTOR_API
	#define VECTOR_API
#endif

#ifdef VECTOR_C
	#define VECTOR_ITEM_TYPE VECTOR_C
	#define VECTOR_NO_HEADERS
	#define VECTOR_IMPLEMENTATION
#endif

#ifndef VECTOR_ITEM_TYPE
	#error VECTOR_ITEM_TYPE is undefined, cannot construct vector.
#else
	#include <string.h>
	
	#ifndef VECTOR_MALLOC_FN
		#include <stdlib.h>
		#define VECTOR_MALLOC_FN malloc
	#endif

	#ifndef VECTOR_FREE_FN
		#include <stdlib.h>
		#define VECTOR_FREE_FN free
	#endif

	#include <better_c_std/weak.h>
	#ifdef VECTOR_MAKE_STATIC
		#define VEC_STATIC_PREFIX static
	#else
		#define VEC_STATIC_PREFIX WEAK
	#endif

	#include <stddef.h>
	#include <better_c_std/prettify/assert.h>
	#include <better_c_std/prettify/misc.h>

	#ifdef VECTOR_C
		#define VEC_T CONCAT(vec_, VECTOR_C)
		#define ITEM_T VECTOR_C
	#else
		#define VEC_T CONCAT(vec_, VECTOR_ITEM_TYPE)
		#define ITEM_T VECTOR_ITEM_TYPE
	#endif

	#ifndef VECTOR_NO_HEADERS
		typedef struct {
			VECTOR_ITEM_TYPE* data;
			size_t length;
			size_t capacity;
		} VEC_T;
	#endif

	// Names of functions
	#define VEC_CREATE            CONCAT(VEC_T, _create)
	#define VEC_WITH_CAPACITY     CONCAT(VEC_T, _with_capacity)
	#define VEC_WITH_CAPACITY_TRY CONCAT(VEC_T, _with_capacity_try)
	#define VEC_CREATE_COPY       CONCAT(VEC_T, _create_copy)
	#define VEC_CLONE             CONCAT(VEC_T, _clone)
	#define VEC_FROM_RAW          CONCAT(VEC_T, _from_raw)
	#define VEC_PUSH              CONCAT(VEC_T, _push)
	#define VEC_INSERT            CONCAT(VEC_T, _insert)
	#define VEC_POPGET            CONCAT(VEC_T, _popget)
	#define VEC_POPFREE           CONCAT(VEC_T, _popfree)
	#define VEC_AT                CONCAT(VEC_T, _at)
	#define VEC_ATREF             CONCAT(VEC_T, _atref)
	#define VEC_EXTRACT_FAST      CONCAT(VEC_T, _extract_fast)
	#define VEC_EXTRACT_ORDER     CONCAT(VEC_T, _extract_order)
	#define VEC_DELETE_FAST       CONCAT(VEC_T, _delete_fast)
	#define VEC_DELETE_ORDER      CONCAT(VEC_T, _delete_order)
	#define VEC_FREE              CONCAT(VEC_T, _free)

	// Function declarations
	#ifndef VECTOR_NO_HEADERS
		VECTOR_API VEC_STATIC_PREFIX VEC_T   VEC_CREATE           ();
		VECTOR_API VEC_STATIC_PREFIX VEC_T   VEC_WITH_CAPACITY    (size_t cap);
		VECTOR_API VEC_STATIC_PREFIX VEC_T   VEC_WITH_CAPACITY_TRY(size_t cap);
		VECTOR_API VEC_STATIC_PREFIX VEC_T   VEC_CREATE_COPY      (const ITEM_T* source, size_t length);
		VECTOR_API VEC_STATIC_PREFIX VEC_T   VEC_CLONE            (const VEC_T* source);
		VECTOR_API VEC_STATIC_PREFIX VEC_T   VEC_FROM_RAW         (ITEM_T* source, size_t length);
		VECTOR_API VEC_STATIC_PREFIX void    VEC_PUSH             (VEC_T* vec, ITEM_T item);
		VECTOR_API VEC_STATIC_PREFIX void    VEC_INSERT           (VEC_T* vec, ITEM_T item, size_t index);
		VECTOR_API VEC_STATIC_PREFIX ITEM_T  VEC_POPGET           (VEC_T* vec);
		VECTOR_API VEC_STATIC_PREFIX void    VEC_POPFREE          (VEC_T* vec);
		VECTOR_API VEC_STATIC_PREFIX ITEM_T  VEC_AT               (VEC_T* vec, size_t i);
		VECTOR_API VEC_STATIC_PREFIX ITEM_T* VEC_ATREF            (VEC_T* vec, size_t i);
		VECTOR_API VEC_STATIC_PREFIX ITEM_T  VEC_EXTRACT_FAST     (VEC_T* vec, size_t i);
		VECTOR_API VEC_STATIC_PREFIX ITEM_T  VEC_EXTRACT_ORDER    (VEC_T* vec, size_t i);
		VECTOR_API VEC_STATIC_PREFIX void    VEC_DELETE_FAST      (VEC_T* vec, size_t i);
		VECTOR_API VEC_STATIC_PREFIX void    VEC_DELETE_ORDER     (VEC_T* vec, size_t i);
		VECTOR_API VEC_STATIC_PREFIX void    VEC_FREE             (VEC_T v);
	#endif

	#ifdef VECTOR_IMPLEMENTATION
		// Implementations
		VECTOR_API VEC_STATIC_PREFIX VEC_T VEC_CREATE() {
			return (VEC_T){
					.data = NULL,
					.length = 0,
					.capacity = 0,
			};
		}

		VECTOR_API VEC_STATIC_PREFIX VEC_T VEC_WITH_CAPACITY_TRY(size_t cap) {
			if (cap == 0)
				return VEC_CREATE();

			VEC_T result;

			result.data = (ITEM_T*)VECTOR_MALLOC_FN(sizeof(ITEM_T) * cap);
			result.length = 0;
			result.capacity = result.data == NULL ? 0 : cap;

			return result;
		}

		VECTOR_API VEC_STATIC_PREFIX VEC_T VEC_WITH_CAPACITY(size_t cap) {
			VEC_T result = VEC_WITH_CAPACITY_TRY (cap);
			assert_m(result.capacity == cap);
			return result;
		}

		VECTOR_API VEC_STATIC_PREFIX VEC_T VEC_CREATE_COPY(const ITEM_T* source, size_t length) {
			VEC_T result;

			result.data = (ITEM_T*)VECTOR_MALLOC_FN(sizeof(ITEM_T) * length);
			#ifdef VECTOR_ITEM_CLONE_FN
				for (size_t i = 0; i < length; i++) {
					result.data[i] = VECTOR_ITEM_CLONE_FN(&source[i]);
				}
			#else
				memcpy(result.data, source, length * sizeof(ITEM_T));
			#endif

			result.length = length;
			result.capacity = length;
			return result;
		}

		VECTOR_API VEC_STATIC_PREFIX VEC_T VEC_CLONE(const VEC_T* source) {
			return VEC_CREATE_COPY(source->data, source->length);
		}

		VECTOR_API VEC_STATIC_PREFIX VEC_T VEC_FROM_RAW(ITEM_T* source, size_t length) {
			return (VEC_T){
					.data = source,
					.length = length,
					.capacity = length,
			};
		}

		#ifdef DEBUG
			#define CHECK_LEN_ZERO(vec)                                       \
				if ((vec)->length <= 0)                                         \
					panic("%s (at %p) is empty, it's length is %zu\n", STR(VEC_T), \
								(void*)(vec), (vec)->length);

			#define CHECK_INDEX(vec, index)                                           \
				if (index >= (vec)->length)                                             \
					panic("%s (at %p) index is out of bounds. Index = %zu, length = %zu\n", \
								STR(VEC_T), (void*)(vec), index, (vec)->length);
		#else
			#define CHECK_LEN_ZERO(vec) \
				if ((vec)->length <= 0) unreachable();

			#define CHECK_INDEX(vec, index) \
				if (index >= (vec)->length) unreachable();
		#endif

		VECTOR_API VEC_STATIC_PREFIX void VEC_PUSH(VEC_T* vec, ITEM_T item) {
			if (vec->length > vec->capacity) {
				#ifdef DEBUG
					panic("Absurd situation: vector length (%zu) is bigger than it's capacity (%zu)\n",
							vec->length, vec->capacity);
				#else
					unreachable();
				#endif
			} else if (vec->length == vec->capacity) {
				size_t new_cap = vec->length * 3 / 2 + 4;
				#ifdef VECTOR_REALLOC_FN
					ITEM_T* new_data =
							(ITEM_T*)VECTOR_REALLOC_FN(vec->data, sizeof(ITEM_T) * new_cap);
				#else
					ITEM_T* new_data = (ITEM_T*)VECTOR_MALLOC_FN(sizeof(ITEM_T) * new_cap);
					memcpy(new_data, vec->data, sizeof(ITEM_T) * vec->length);
					VECTOR_FREE_FN(vec->data);
				#endif

				vec->data = new_data;
				vec->capacity = new_cap;
			}

			vec->data[vec->length] = item;
			vec->length++;
		}

		VECTOR_API VEC_STATIC_PREFIX void VEC_INSERT(VEC_T* vec, ITEM_T item, size_t index) {
			if (index == vec->length) {
				VEC_PUSH(vec, item);
			} else {
				CHECK_INDEX(vec, index);
				ITEM_T tmp = vec->data[index];
				vec->data[index] = item;
				for (size_t i = index + 1; i < vec->length; i++) {
					ITEM_T tmp2 = vec->data[i];
					vec->data[i] = tmp;
					tmp = tmp2;
				}
				VEC_PUSH(vec, tmp);
			}
		}

		VECTOR_API VEC_STATIC_PREFIX ITEM_T VEC_POPGET(VEC_T* vec) {
			CHECK_LEN_ZERO(vec);
			vec->length--;
			return vec->data[vec->length];
		}

		VECTOR_API VEC_STATIC_PREFIX void VEC_POPFREE(VEC_T* vec) {
			CHECK_LEN_ZERO(vec);
			vec->length--;

			#ifdef VECTOR_ITEM_DESTRUCTOR
				VECTOR_ITEM_DESTRUCTOR(vec->data[vec->length]);
			#endif
		}

		VECTOR_API VEC_STATIC_PREFIX ITEM_T VEC_AT(VEC_T* vec, size_t i) {
			CHECK_INDEX(vec, i);
			return vec->data[i];
		}
		VECTOR_API VEC_STATIC_PREFIX ITEM_T* VEC_ATREF(VEC_T* vec, size_t i) {
			CHECK_INDEX(vec, i);
			return &vec->data[i];
		}

		VECTOR_API VEC_STATIC_PREFIX ITEM_T VEC_EXTRACT_FAST(VEC_T* vec, size_t i) {
			CHECK_LEN_ZERO(vec);
			CHECK_INDEX(vec, i);

			ITEM_T item = vec->data[i];
			vec->data[i] = vec->data[vec->length - 1];
			vec->length--;
			return item;
		}

		VECTOR_API VEC_STATIC_PREFIX ITEM_T VEC_EXTRACT_ORDER(VEC_T* vec, size_t i) {
			CHECK_LEN_ZERO(vec);
			CHECK_INDEX(vec, i);

			ITEM_T item = vec->data[i];
			for (size_t index = i; index < vec->length - 1; index++)
				vec->data[index] = vec->data[index + 1];
			vec->length--;
			return item;
		}

		VECTOR_API VEC_STATIC_PREFIX void VEC_DELETE_FAST(VEC_T* vec, size_t i) {
			CHECK_LEN_ZERO(vec);
			CHECK_INDEX(vec, i);

			#ifdef VECTOR_ITEM_DESTRUCTOR
				VECTOR_ITEM_DESTRUCTOR(vec->data[i]);
			#endif
			vec->data[i] = vec->data[vec->length - 1];
			vec->length--;
		}
		VECTOR_API VEC_STATIC_PREFIX void VEC_DELETE_ORDER(VEC_T* vec, size_t i) {
			CHECK_LEN_ZERO(vec);
			CHECK_INDEX(vec, i);

			#ifdef VECTOR_ITEM_DESTRUCTOR
				VECTOR_ITEM_DESTRUCTOR(vec->data[i]);
			#endif

			vec->length--;
			for (size_t index = i; index < vec->length; index++) {
				vec->data[index] = vec->data[index + 1];
			}
		}

		VECTOR_API VEC_STATIC_PREFIX void VEC_FREE(VEC_T v) {
			#ifdef VECTOR_ITEM_DESTRUCTOR
				for (size_t i = 0; i < v.length; i++) VECTOR_ITEM_DESTRUCTOR(v.data[i]);
			#endif

			if (v.data) VECTOR_FREE_FN(v.data);

			#ifdef DEBUG
				printf("Deleted " STR(VEC_T) " at <?> (length %zu)\n", v.length);
			#endif
		}

		#undef CHECK_LEN_ZERO
		#undef CHECK_INDEX
	#endif

	#undef VEC_CREATE
	#undef VEC_WITH_CAPACITY
	#undef VEC_WITH_CAPACITY_TRY
	#undef VEC_CREATE_COPY
	#undef VEC_CLONE
	#undef VEC_FROM_RAW
	#undef VEC_PUSH
	#undef VEC_INSERT
	#undef VEC_POPGET
	#undef VEC_POPFREE
	#undef VEC_AT
	#undef VEC_ATREF
	#undef VEC_EXTRACT_FAST
	#undef VEC_EXTRACT_ORDER
	#undef VEC_DELETE_FAST
	#undef VEC_DELETE_ORDER
	#undef VEC_FREE

	#undef VECTOR_ITEM_TYPE
	#undef VEC_STATIC_PREFIX
	#undef VEC_T
	#undef ITEM_T
#endif

#undef VECTOR_NO_HEADERS
#undef VECTOR_IMPLEMENTATION
#undef VECTOR_MAKE_STATIC
#undef VECTOR_C


#pragma GCC diagnostic pop