#ifndef PAGING_H_L4JMLPEK
#define PAGING_H_L4JMLPEK

#include <mm/types.h>
#include <types.h>

#define PAGE_DIR_SIZE   1024
#define PAGE_TAB_SIZE   1024

/**
 * Setup and initialize paging.
 */
void paging_init();

/**
 * Pointer to the paging directory
 */
extern struct page_directory_entry *page_directory;

/**
 * Load the given page directory. Implemented in asm.
 *
 * @param   uintptr_t * Address of page directory
 */
extern void page_directory_load(uintptr_t *);

/**
 * Enable the paging. Implemented in asm.
 */
extern void paging_enable();

/**
 * Map a page in paging directory/table. This assumes that the physical address
 * is already allocated by page_frame_alloc().
 *
 * @param   void *          Physical address of the page
 * @param   void *          Virtual address of the page
 * @param   uint32_t        Flags for the entry
 * @return  int             0 if successful, 1 if mapping already present
 */
int paging_map_page(void *, void *, uint32_t);

/**
 * Unmap a page in the paging directory/table. This assumes that the physical
 * address if deallocated by page_frame_free().
 *
 * @param   void *          Virtual address to unmap
 */
void paging_unmap_page(void *);

#ifdef DEBUG
/**
 * Dump the paging directory structures
 */
void paging_dump_map(void);
#endif

#endif /* end of include guard: PAGING_H_L4JMLPEK */
