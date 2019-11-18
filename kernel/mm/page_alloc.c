
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

static struct page *pages = NULL;

void page_alloc_init()
{
  klog_status_init("page_alloc");

  size_t array_mem_size = sizeof(struct page) * memory_info->num_pages;
  array_mem_size = (array_mem_size & 0xfffff000) + 0x1000;
  pages = page_frame_n_alloc(array_mem_size >> 12);

  klog_status_ok("page_alloc");
}

inline struct page * page_alloc()
{
  return page_n_alloc(1);
}

struct page * page_n_alloc(size_t count)
{
  void *page_address = page_frame_n_alloc(count);
  size_t page_index = (uintptr_t)page_address >> 12;

  for (size_t i = 0; i < count; i++) {
    pages[page_index + i].address = page_address;
    page_address += 0x1000;
  }

  return &pages[page_index];
}

inline void page_free(struct page *page)
{
  page_n_free(page, 1);
}

void page_n_free(struct page *page, size_t count)
{
  page_frame_n_free(page->address, count);
  for (size_t i = 0; i < count; i++) {
    assert(page[i].ref_count == 0);
    assert(page[i].address != NULL);
    page[i].address = NULL;
  }
}
