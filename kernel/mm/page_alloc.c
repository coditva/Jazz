
/*
 * This is a wrapper over page_frame. It takes care of allocating/de-allocating
 * chunks of pages from the physical memory. It wraps all allocated memory in
 * the `struct page` data structure.
 */

#include <assert.h>
#include <logger.h>
#include <mm/mem_info.h>
#include <mm/page_alloc.h>
#include <mm/page_frame.h>
#include <mm/types.h>
#include <types.h>

static inline size_t page_address_to_index(uintptr_t page_address)
{
  return page_address >> 12;
}

static struct page *pages = NULL;

void page_alloc_init()
{
  klog_status_init("page_alloc");

  /* get the size of array which will be used to store all the pages */
  size_t pages_array_mem_size = sizeof(struct page) * memory_info->num_pages;

  /* get the number of pages that will be used to store this array */
  size_t pages_array_page_count =
    page_address_to_index(pages_array_mem_size) + 1;

  /* allocate the pages for the array */
  pages = page_frame_n_alloc(pages_array_page_count);

  klog_status_ok("page_alloc");
}

struct page *page_n_alloc(size_t count)
{
  void * page_address = page_frame_n_alloc(count);
  size_t page_index   = page_address_to_index((uintptr_t)page_address);

  for (size_t i = 0; i < count; i++) {
    pages[page_index + i].address = page_address;
    page_address += 0x1000;
  }

  return &pages[page_index];
}

inline struct page *page_alloc()
{
  return page_n_alloc(1);
}

void page_n_free(struct page *page, size_t count)
{
  for (size_t i = 0; i < count; i++) {
    /* ensure that there are no references to the page before freeing */
    assert(page[i].ref_count == 0);

    /* ensure that page was not already freed */
    assert(page[i].address != NULL);

    page[i].address = NULL;
  }

  /* finally free the frames */
  page_frame_n_free(page->address, count);
}

inline void page_free(struct page *page)
{
  page_n_free(page, 1);
}
