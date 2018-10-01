#ifndef MEMORY_ALLOC_H_ZPNDLSE1
#define MEMORY_ALLOC_H_ZPNDLSE1

#include <types.h>

/**
 * Allocate memory of given size and return its starting address.
 *
 * @param   size_t      size of the memory to allocate
 * @return  void *      address of the memory. NULL if allocation failed
 */
void *malloc(size_t);

/**
 * Reallocate a piece of memory with the new size.
 *
 * @param   void *      address of memory to reallocate
 * @param   size_t      size of the new memory
 * @return  void *      address of the newly reallocated memory. NULL if
 *                      reallocation failed.
 */
void *realloc(void *, size_t);

/**
 * Allocates memory for the given number of array elements of the given array
 * size. The memory is set to 0.
 *
 * @param   size_t      size of the array to allocate memory for
 * @param   size_t      size of each array element
 * @return  void *      address of the newly allocated memory. NULL if
 *                      allocation failed.
 */
void *calloc(size_t, size_t);

/**
 * Free a piece of allocated memory.
 *
 * @param   void *      address of the allocated memory
 */
void free(void *);

#endif /* end of include guard: MEMORY_ALLOC_H_ZPNDLSE1 */
