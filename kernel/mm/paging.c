
/*
 * This is a low level module for handling paging. It has routines to map/unmap
 * virtual addresses to physical addresses.
 */

#include <assert.h>
#include <logger.h>
#include <mm/page_frame.h>
#include <mm/page_alloc.h>
#include <mm/paging.h>
#include <mm/types.h>
#include <string.h>
#include <types.h>

#define TABLE_TO_ADDRESS_MASK 0xfffff000

/**
 * The global page directory entry
 */
struct page_directory_entry *page_directory = NULL;

/**
 * The start and end addresses for the kernel
 * Defined in the linker.d file
 */
extern uintptr_t startkernel;
extern uintptr_t endkernel;

static inline void *table_to_address(uintptr_t value)
{
  return (void *)(value & TABLE_TO_ADDRESS_MASK);
}

/**
 * Get the value of the page table from the directory
 *
 * @param   struct page_directory_entry *       The page directory
 * @param   struct page_table_entry *           The page table
 */
static inline struct page_table_entry *
page_directory_get_value(struct page_directory_entry *page_dir_entry)
{
  return (struct page_table_entry *)page_dir_entry->value;
}

/**
 * Set the value of the page table in the directory
 *
 * @param   struct page_directory_entry *       The page directory
 * @param   struct page_table_entry *           The page table
 */
static inline void
page_directory_set_value(struct page_directory_entry *page_dir_entry,
                         struct page_table_entry *    value)
{
  page_dir_entry->value = (uintptr_t)value;
}

/**
 * Get the value of the page table
 *
 * @param   struct page_table_entry *           The page table
 */
static inline uint32_t
page_table_get_value(struct page_table_entry *page_tab_entry)
{
  return page_tab_entry->value;
}

/**
 * Set the value of the page table
 *
 * @param   struct page_table_entry *           The page table
 * @param   uint32_t                            The value to be set
 */
static inline void page_table_set_value(struct page_table_entry *page_tab_entry,
                                        uint32_t                 value)
{
  page_tab_entry->value = value;
}

/**
 * Returns the address of the page table for the table entry
 *
 * @param   struct page_directory_entry *       The directory entry
 * @return  void *                              The address of page
 */
static inline void *
page_table_to_address(struct page_table_entry *page_table_entry)
{
  return table_to_address((uintptr_t)page_table_entry->value);
}

/**
 * Returns the address of the page table for the directory entry
 *
 * @param   struct page_directory_entry *       The directory entry
 * @return  void *                              The address of page
 */
static inline void *
page_directory_to_address(struct page_directory_entry *page_dir_entry)
{
  return table_to_address((uintptr_t)page_dir_entry->value);
}

static inline void create_new_directory(int directory_index)
{
  /* if page dir is not present, create a page directory and fill it with
   * empty page tables */
  assert(!page_directory[directory_index].present);

  struct page_table_entry *page_table = page_frame_alloc();

  /* set all the tables to 0 */
  memset(page_table, 0, PAGE_TAB_SIZE * sizeof(struct page_table_entry));

  /* add the tables to the directory */
  page_directory_set_value(&page_directory[directory_index], page_table);
  page_directory[directory_index].rw      = 1;
  page_directory[directory_index].present = 1;

  klog(LOG_DEBUG, "paging_create_new_directory: new directory entry: 0x%x\n",
       page_directory_get_value(&page_directory[directory_index]));
}

void paging_init()
{
  klog_status_init("paging");

  /* create an empty page directory */
  page_directory = page_frame_alloc();
  for (int i = 0; i < PAGE_DIR_SIZE; i++) {
    /* Sets flags = kernel-mode, write enabled, not-present (empty) */
    page_directory[i].is_user = 0;
    page_directory[i].rw      = 1;
    page_directory[i].present = 0;
  }

  /* map the first few pages (this includes the kernel pages too) */
  struct page_table_entry *page_table = page_frame_alloc();
  for (int i = 0; i < PAGE_TAB_SIZE; i++) {
    page_table_set_value(&page_table[i], (i * PAGE_SIZE) | 3);
  }

  /* add the table to the directory */
  page_directory_set_value(&page_directory[0], page_table);
  page_directory[0].present = 1;

  page_directory_load((void *)page_directory);
  paging_enable();
  klog_status_ok("paging");
}

int paging_map_page(struct page *page, void *virtual_address, uint32_t flags)
{
  uint32_t                 page_dir_index = 0;
  uint32_t                 page_tab_index = 0;
  struct page_table_entry *page_table     = NULL;

  klog(LOG_DEBUG, "paging_map_page: mapping 0x%x to 0x%x\n", virtual_address,
       page->address);

  /* ensure addresses are 4kb aligned */
  assert(((uintptr_t)page->address & 0xfff) == 0);
  assert(((uintptr_t)virtual_address & 0xfff) == 0);

  // TODO(coditva): magic num
  /* get the table for the virtual address requested */
  page_dir_index = (uintptr_t)virtual_address >> 22;
  page_tab_index = (uintptr_t)virtual_address >> 12 & 0x3FF;

  /* if page dir is not present, create a page directory and fill it with
   * empty page tables */
  if (!page_directory[page_dir_index].present) {
    create_new_directory(page_dir_index);
  }

  /* get the page from the directory */
  page_table = page_directory_to_address(&page_directory[page_dir_index]);

  /* ensure that page does not exist already */
  assert(!page_table[page_tab_index].present);

  /* try adding a new page table entry */
  page_table_set_value(&page_table[page_tab_index],
                       (uintptr_t)page->address | (flags & 0xfff));
  page_table[page_tab_index].present = 1;

  klog(LOG_DEBUG, "paging_map_page: new table entry: 0x%x\n",
       page_table_get_value(&page_table[page_tab_index]));

  /* increment the number of references for the page */
  page->ref_count += 1;

  return 0;
}

void paging_unmap_page(struct page *page, void *virtual_address)
{
  klog(LOG_DEBUG, "paging_unmap_page: unmapping 0x%x\n", virtual_address);

  /* there should be at-least 1 reference for the page */
  assert(page->ref_count > 0);

  struct page_table_entry *page_table = NULL;

  // TODO(coditva): magic num
  uintptr_t page_dir_index = (uintptr_t)virtual_address >> 22;
  uintptr_t page_tab_index = (uintptr_t)virtual_address >> 12 & 0x3FF;

  /* ensure that the page directory is present */
  assert(page_directory[page_dir_index].present);

  /* get the table from the directory */
  page_table = (struct page_table_entry *)page_directory_to_address(
    &page_directory[page_dir_index]);

  /* ensure that page table is present */
  assert(page_table[page_tab_index].present);

  /* reset the table */
  page_table_set_value(&page_table[page_tab_index], 0x0);

  /* decrement the reference count */
  page->ref_count -= 1;
}

#ifdef DEBUG
void paging_dump_map(void)
{
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "PAGING MEMORY DUMP START\n");
  klog(LOG_DEBUG, LOG_HRULE);

  for (uint32_t i = 0; i < PAGE_DIR_SIZE; i++) {
    struct page_directory_entry *page_dir = &page_directory[i];
    if (!page_dir->present) {
      continue;
    }
    struct page_table_entry *page_table = page_directory_to_address(page_dir);
    klog(LOG_DEBUG, "page dir at 0x%x\n", page_table);
    for (uint32_t j = 0; j < PAGE_TAB_SIZE; j++) {
      klog(LOG_DEBUG, "- page table entry: 0x%x\n", page_table[j]);
    }
  }

  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "PAGING MEMORY DUMP END\n");
  klog(LOG_DEBUG, LOG_HRULE);
}
#endif
