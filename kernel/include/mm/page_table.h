#ifndef PAGE_TABLE_H_Q7YW1FHJ
#define PAGE_TABLE_H_Q7YW1FHJ

#include <types.h>

/**
 * Get the address of the page table with the given index.
 *
 * @param   uintptr_t   Index of the page directory to get from
 * @param   uintptr_t   Index of the page table
 * @return  void *      Address of the page table. NULL if not present
 */
void * page_table_get(uintptr_t, uintptr_t);

/**
 * Add a new page table entry to the page table.
 *
 * @param   uintptr_t   Index of the page directory to add to
 * @param   uintptr_t   Index of the new page table
 * @param   void *      Physical address of the new page entry
 * @param   uint32_t    Flags for the new page table entry
 * @return  void *      Address of the newly added page table. NULL if already
 *                      present
 */
void * page_table_add(uintptr_t, uintptr_t, void *, uint32_t);

/**
 * Remove a page table entry from the page table
 *
 * @param   uintptr_t   Index of the page directory
 * @param   uintptr_t   Index of the page table
 */
void page_table_remove(uintptr_t, uintptr_t);

#endif /* end of include guard: PAGE_TABLE_H_Q7YW1FHJ */
