#include <helper.h>
#include <mm/page_table.h>
#include <types.h>

void * page_table_get(uintptr_t page_dir_index, uintptr_t index)
{
  MARK_PARAM_USED(page_dir_index);
  MARK_PARAM_USED(index);
  return NULL;
}

void * page_table_add(uintptr_t page_dir_index, uintptr_t index,
    void *physical_address, uint32_t flags)
{
  MARK_PARAM_USED(index);
  MARK_PARAM_USED(page_dir_index);
  MARK_PARAM_USED(physical_address);
  MARK_PARAM_USED(flags);
  return NULL;
}

void page_table_remove(uintptr_t page_dir_index, uintptr_t index)
{
  MARK_PARAM_USED(page_dir_index);
  MARK_PARAM_USED(index);
}
