#ifndef PAGE_DIRECTORY_H_ITHVGZCU
#define PAGE_DIRECTORY_H_ITHVGZCU

#include <types.h>

/**
 * Load the given page directory. Implemented in asm.
 *
 * @param   uintptr_t * Address of page directory
 */
extern void page_directory_load(uintptr_t *);

/**
 * Get the address of the page directory identified by index.
 *
 * @param   uintptr_t   Index of the directory
 * @return  void *      Address of page directory. NULL if not present.
 */
void * page_directory_get(uintptr_t);

/**
 * Add a new page directory at the given index and return its address
 *
 * @param   uintptr_t   Index of the new directory
 * @return  void *      Address of the newly added directory. NULL if already
 *                      present.
 */
void * page_directory_add(uintptr_t);

#endif /* end of include guard: PAGE_DIRECTORY_H_ITHVGZCU */
