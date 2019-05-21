#include <logger.h>
#include <mm/page_directory.h>
#include <mm/page_frame.h>
#include <mm/page_table.h>
#include <mm/paging.h>
#include <types.h>

uintptr_t *page_directory = NULL;

extern uintptr_t startkernel;
extern uintptr_t endkernel;

void paging_init()
{
  klog_status_init("paging");

  /* create an empty page directory */
  page_directory = page_frame_alloc();
  for (uint32_t i = 0; i < PAGE_DIR_SIZE; i++) {
    /* Sets flags = kernel-mode, write enabled, not-present (empty) */
    page_directory[i] = 0x00000002;
  }

  /* map the first few pages (this includes the kernel pages too) */
  uint32_t *page_table = page_frame_alloc();
  for (uint32_t i = 0; i < PAGE_TAB_SIZE; i++) {
    page_table[i] = (i * 0x1000) | 3;
  }
  page_directory[0] = (uintptr_t)page_table | 3;

#ifdef DEBUG
  paging_dump_map();
#endif

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
  if ((uintptr_t)physical_address & 0xfff) {
    klog(LOG_ERROR, "paging_map_page: physical address 0x%x not aligned\n",
        physical_address);
    return 1;
  }
  if ((uintptr_t)virtual_address & 0xfff) {
    klog(LOG_ERROR, "paging_map_page: virtual address 0x%x not aligned\n",
        virtual_address);
    return 1;
  }

  page_dir_index = (uintptr_t)virtual_address >> 22;
  page_tab_index =
    (uintptr_t)virtual_address >> 12 & 0x3FF; // TODO(coditva): magic num

  /* check if page dir is present, if not create a new page dir and add it */
  if (page_directory_add(page_dir_index) == NULL) {
    klog(LOG_DEBUG, "paging_map_page: page directory already present\n");
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

#ifdef DEBUG
void paging_dump_map(void)
{
  for (uint32_t i = 0; i < PAGE_DIR_SIZE; i++) {
    uint32_t *page_table = (void *)page_directory[i];
    if (((uintptr_t)page_table & 0x00000001) == 0) {
      klog(LOG_DEBUG, "no page table at 0x%x\n", &page_directory[i]);
      continue;
    }
    page_table = (void *)((uintptr_t)page_table & 0xfffff000);
    klog(LOG_DEBUG, "page page at 0x%x\n", page_table);
    for (uint32_t j = 0; j < PAGE_TAB_SIZE; j++) {
      klog(LOG_DEBUG, "-- page table entry: 0x%x\n", page_table[j]);
    }
  }
}
#endif
