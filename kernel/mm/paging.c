#include <logger.h>
#include <mm/page_frame.h>
#include <mm/paging.h>
#include <mm/types.h>
#include <types.h>
#include <assert.h>
#include <string.h>

struct page_directory_entry *page_directory = NULL;

extern uintptr_t startkernel;
extern uintptr_t endkernel;

static inline void * 
page_directory_to_address(struct page_directory_entry *page_dir_entry)
{
  return (void *)((uintptr_t)page_dir_entry->value & 0xfffff000);
}

static inline uint32_t
page_directory_get_value(struct page_directory_entry *page_dir_entry)
{
  return page_dir_entry->value;
}

static inline void
page_directory_set_value(struct page_directory_entry *page_dir_entry,
    uint32_t value)
{
  page_dir_entry->value = value;
}

static inline uint32_t
page_table_get_value(struct page_table_entry *page_tab_entry)
{
  return page_tab_entry->value;
}

static inline void
page_table_set_value(struct page_table_entry *page_tab_entry,
    uint32_t value)
{
  page_tab_entry->value = value;
}

void paging_init()
{
  klog_status_init("paging");

  /* create an empty page directory */
  page_directory = page_frame_alloc();
  for (uint32_t i = 0; i < PAGE_DIR_SIZE; i++) {
    /* Sets flags = kernel-mode, write enabled, not-present (empty) */
    page_directory[i].is_user = 0;
    page_directory[i].rw = 1;
    page_directory[i].present = 0;
  }

  /* map the first few pages (this includes the kernel pages too) */
  uint32_t *page_table = page_frame_alloc();
  for (uint32_t i = 0; i < PAGE_TAB_SIZE; i++) {
    page_table[i] = (i * 0x1000) | 3;
  }

  page_directory_set_value(&page_directory[0], (uintptr_t)page_table);
  page_directory[0].rw = 1;
  page_directory[0].present = 1;

#ifdef DEBUG
  paging_dump_map();
#endif

  page_directory_load((void *)page_directory);
  paging_enable();
  klog_status_ok("paging");
}

int paging_map_page(void *physical_address, void *virtual_address,
    uint32_t flags)
{
  uintptr_t page_dir_index;
  uintptr_t page_tab_index;
  struct page_table_entry *page_table;

  klog(LOG_DEBUG, "paging_map_page: mapping 0x%x to 0x%x\n",
      virtual_address, physical_address);

  /* check if addresses are 4kb aligned */
  assert(((uintptr_t)physical_address & 0xfff) == 0);
  assert(((uintptr_t)virtual_address & 0xfff) == 0);

  page_dir_index = (uintptr_t)virtual_address >> 22;
  page_tab_index =
    (uintptr_t)virtual_address >> 12 & 0x3FF; // TODO(coditva): magic num

  /* if page dir is not present, create a page directory and fill it with
   * empty page tables */
  if (!page_directory[page_dir_index].present) {
    page_table = page_frame_alloc();
    memset(page_table, 0, PAGE_TAB_SIZE * sizeof(struct page_table_entry));

    page_directory_set_value(&page_directory[page_dir_index], (uintptr_t)page_table);
    page_directory[page_dir_index].rw = 1;
    page_directory[page_dir_index].present = 1;
    klog(LOG_DEBUG, "new directory entry: 0x%x\n",
        page_directory_get_value(&page_directory[page_dir_index]));
  }

  page_table = (struct page_table_entry *)
      page_directory_to_address(&page_directory[page_dir_index]);
  assert(!page_table[page_tab_index].present);

  /* try adding a new page table entry, if it is already present do nothing and
   * return. */
  page_table_set_value(&page_table[page_tab_index],
      (uintptr_t)physical_address | (flags & 0xfff));
  page_table[page_tab_index].present = 1;
  klog(LOG_DEBUG, "new table entry: 0x%x\n",
      page_table_get_value(&page_table[page_tab_index]));

  return 0;
}

void paging_unmap_page(void *virtual_address)
{
  struct page_table_entry *page_table;
  uintptr_t page_dir_index = (uintptr_t)virtual_address >> 22;
  uintptr_t page_tab_index =
    (uintptr_t)virtual_address >> 12 & 0x3FF; // TODO(coditva): magic num

  page_table = (struct page_table_entry *)
      page_directory_to_address(&page_directory[page_dir_index]);
  assert(page_table[page_tab_index].present);
  page_table_set_value(&page_table[page_tab_index], 0x0);
}

#ifdef DEBUG
void paging_dump_map(void)
{
  for (uint32_t i = 0; i < PAGE_DIR_SIZE; i++) {
    uint32_t *page_table = page_directory_to_address(&page_directory[i]);
    if (((uintptr_t)page_table & 0x00000001) == 0) {
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
