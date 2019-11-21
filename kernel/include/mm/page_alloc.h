#ifndef PAGE_ALLOC_C_YUK9OBSD
#define PAGE_ALLOC_C_YUK9OBSD

#include <mm/types.h>
#include <types.h>

/**
 * Initialize page_alloc.
 */
void page_alloc_init();

/**
 * Allocate a physical page from memory.
 *
 * @return  struct page *   Allocated page
 */
struct page *page_alloc();

/**
 * Allocate n number of contiguous pages from memory.
 *
 * @param   size_t          Number of pages to allocate
 * @return  struct page *   First allocated page
 */
struct page *page_n_alloc(size_t);

/**
 * Free an allocated page in memory.
 *
 * @param   struct page *   The page to free
 */
void page_free(struct page *);

/**
 * Free n allocated contiguous pages in memory.
 *
 * @param   struct page *   First page in n contiguous pages
 * @return  size_t          Number of pages to free
 */
void page_n_free(struct page *, size_t);

#endif /* end of include guard: PAGE_ALLOC_C_YUK9OBSD */
