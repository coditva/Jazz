#include <logger.h>
#include <page_directory.h>
#include <page_frame.h>
#include <page_table.h>
#include <paging.h>
#include <types.h>

#define PAGE_DIR_SIZE   1024
#define PAGE_TAB_SIZE   1024

uintptr_t *page_directory = NULL;

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
  uintptr_t *page_table = page_frame_alloc();
  for (uint32_t i = 0; i < PAGE_TAB_SIZE; i++) {
    page_table[i] = (i << 12) | 3;
  }

  /* add first page to paging directory and load it */
  page_directory[0] = ((uintptr_t)page_table) | 3;
  page_directory_load(page_directory);

  paging_enable();
  klog_status_ok("paging");
}

int paging_map_page(void *physical_address, void *virtual_address,
    uint32_t flags)
{
  uintptr_t page_dir_index;
  uintptr_t page_tab_index;

  /* check if addresses are aligned */
  if ((uintptr_t)physical_address & ~0x1000) {
    klog(LOG_ERROR, "paging_map_page: physical address not aligned");
  }
  if ((uintptr_t)virtual_address & ~0x1000) {
    klog(LOG_ERROR, "paging_map_page: virtual address not aligned");
  }

  page_dir_index = (uintptr_t)virtual_address >> 22;
  page_tab_index =
    (uintptr_t)virtual_address >> 12 & 0x3FF; // TODO(coditva): magic num

  /* check if page dir is present, if not create a new page dir and add
   * it */
  if (page_directory_get(page_dir_index) == NULL) {
    page_directory_add(page_dir_index);
  }

  /* try adding a new page table entry, if it is already present do nothing and
   * return. */
  if (page_table_add(page_dir_index, page_tab_index, physical_address, flags)
      == NULL) {
    klog(LOG_ERROR, "paging_map_page: page table already present\n");
    return 1;
  }

  return 0;
}

void paging_unmap_page(void *virtual_address)
{
  uintptr_t page_dir_index = (uintptr_t)virtual_address >> 22;
  uintptr_t page_tab_index =
    (uintptr_t)virtual_address >> 12 & 0x3FF; // TODO(coditva): magic num

  page_table_remove(page_dir_index, page_tab_index);
}
