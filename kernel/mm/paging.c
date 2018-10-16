#include <logger.h>
#include <page_frame.h>
#include <paging.h>
#include <types.h>

extern void page_directory_load(uint32_t *);
extern void paging_enable();

#define PAGE_DIR_SIZE   1024
#define PAGE_TAB_SIZE   1024

uint32_t *page_directory = NULL;

void paging_init()
{
  klog_status_init("paging");

  /* create an empty page directory */
  page_directory = page_frame_alloc();
  for (uint32_t i = 0; i < PAGE_DIR_SIZE; i++) {
    /* Sets flags = kernel-mode, write enabled, not-present (empty) */
    page_directory[i] = 0x00000002;
  }

  /* create an empty page table */
  uint32_t *page_table = page_frame_alloc();
  for (uint32_t i = 0; i < PAGE_TAB_SIZE; i++) {
    page_table[i] = (i << 12) | 3;
  }

  /* add first page to paging directory and load it */
  page_directory[0] = ((uintptr_t)page_table) | 3;
  page_directory_load(page_directory);

  paging_enable();
  klog_status_ok("paging");
}
