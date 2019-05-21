#include <mm/page_directory.h>
#include <mm/page_table.h>
#include <types.h>

inline void * page_table_get(uintptr_t page_dir_index, uintptr_t index)
{
  uint32_t *page_dir = page_directory_get(page_dir_index);
  if (page_dir[index] & 0x00000000) { /* entry not present */
    return NULL;
  }

  return (void *)((uintptr_t)&page_dir[index] & 0xfffff000);
}

inline void * page_table_add(uintptr_t page_dir_index, uintptr_t index,
    void *physical_address, uint32_t flags)
{
  uint32_t *page_dir = page_directory_get(page_dir_index);

  if (page_dir[index] & 0x00000001) { /* entry already present */
    return NULL;
  }

  page_dir[index] = ((uintptr_t)physical_address | (flags & 0xfff) | 0x01);
  return (void *)((uintptr_t)&page_dir[index] & 0xfffff000);
}

inline void page_table_remove(uintptr_t page_dir_index, uintptr_t index)
{
  uint32_t *page_dir = page_directory_get(page_dir_index);
  page_dir[index] = 0x0;
}
