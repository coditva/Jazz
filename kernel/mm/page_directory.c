#include <mm/page_directory.h>
#include <mm/page_frame.h>
#include <mm/paging.h>
#include <types.h>

inline void * page_directory_get(uintptr_t index)
{
  if (page_directory[index] & 0x00000001) {
    return (void *)(page_directory[index] & 0xfffff000);
  }
  return NULL;
}

inline void * page_directory_add(uintptr_t index)
{
  if (page_directory[index] & 0x00000001) {
    return NULL;
  }

  uint32_t *page_table_entry = page_frame_alloc();
  for (uint32_t i = 0; i < PAGE_TAB_SIZE; i++) {
    page_table_entry[i] = 0x00000000;
  }
  page_directory[index] = (uint32_t)((uintptr_t)page_table_entry) | 3;
  return (void *)(page_directory[index] & 0xfffff000);
}
