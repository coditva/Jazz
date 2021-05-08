#ifndef PAGE_FRAME_ALLOC_H_S1KP2ELH
#define PAGE_FRAME_ALLOC_H_S1KP2ELH

#include <multiboot.h>
#include <types.h>

/* size of each frame that is allocated in bytes */
#define FRAME_SIZE 4096 /* 4Kb */

/**
 * Initializes the memory frames bitmap, which will be used later by alloc/free
 * functions. It also allocates some pages to store the said bitmap.
 *
 * @param void *            base address where kernel ends
 * @param multiboot_into_t  multiboot info
 */
void page_frame_init(void *, multiboot_info_t *);

/**
 * Allocate a page from memory and return it's address.
 *
 * @return void *           Address of the allocated page
 */
void *page_frame_alloc(void);

/**
 * Allocate a number of contiguous pages from memory and return the address of
 * the first page.
 *
 * @param  int              Number of pages to allocate
 * @return void *           Address of the allocated page
 */
void *page_frame_n_alloc(int);

/**
 * Free an allocated page from memory.
 *
 * @param void *            Address of an allocated page
 */
void page_frame_free(void *);

/**
 * Free a number of contiguously allocated pages from memory.
 *
 * @param void *            Address of an allocated page
 * @param int               Number of pages to free
 */
void page_frame_n_free(void *, int);

#ifdef DEBUG
/**
 * Dump the map of page frame on the debug log
 */
void page_frame_dump_map(void);
#endif

#endif /* end of include guard: PAGE_FRAME_ALLOC_H_S1KP2ELH */
