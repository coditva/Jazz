#ifndef PAGE_ALLOC_C_YUK9OBSD
#define PAGE_ALLOC_C_YUK9OBSD

#include <mm/types.h>
#include <types.h>

void            page_alloc_init();
struct page *   page_alloc();
struct page *   page_n_alloc(size_t);
void            page_free(struct page *);
void            page_n_free(struct page *, size_t);

#endif /* end of include guard: PAGE_ALLOC_C_YUK9OBSD */
