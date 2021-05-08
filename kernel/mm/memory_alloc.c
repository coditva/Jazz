
#include <assert.h>
#include <mm/page_alloc.h>
#include <mm/memory_alloc.h>
#include <string.h>

typedef struct block_t
{
  struct block_t *prev;
  struct block_t *next;
  size_t           size;
} __attribute__((packed)) block_t;

typedef struct block_info_t
{
  size_t size;
} __attribute__((packed)) block_info_t;

/**
 * Store the list of free blocks
 */
block_t *free_mem_list = NULL;

/**
 * Allocate a new page, wrap it in a block and return it
 *
 * @return  block_t *   A new free block
 */
static inline block_t *allocate_new_block()
{
  struct page *page  = page_alloc();
  block_t *    block = page->address;

  block->prev = NULL;
  block->next = NULL;
  block->size = PAGE_SIZE_BYTES;

  return block;
}

/**
 * Find a block that can fix the given size request. If a block does not
 * already exist, it is created.
 *
 * @param   size_t      The requested size in bytes
 * @return  block_t *   The block which can fit this size
 */
static inline block_t *find_block(size_t size)
{
  /* requested size should always be less than block size (for now) */
  assert(size <= PAGE_SIZE_BYTES);

  /* if we don't have any blocks left, get a new block and return it */
  if (free_mem_list == NULL) {
    free_mem_list = allocate_new_block();

    return free_mem_list;
  }

  block_t *block = free_mem_list;
  int      found = 0;

  while (1) {
    if (block->size >= size) {
      found = 1;
      break;
    }

    /* ensure that reference to block is always valid */
    if (block->next == NULL) {
      break;
    }

    block = block->next;
  }

  if (!found) {
    /*
     * this block would always be the last block in the list. just update the
     * `next` for it
     */
    block->next       = allocate_new_block();
    block->next->prev = block;

    /* set the new block as the current block */
    block = block->next;
  }

  /* this block would definitely be bigger than or equal to the requested size
   * (see `assert` at the start) */
  return block;
}

/**
 * Free a block and add it to the free_mem_list.
 *
 * @param   block_info_t *      The block info
 */
static inline void free_block(block_info_t *info)
{
  size_t   size  = info->size;
  block_t *block = (void *)info;

  /* add the block to the head of the free_mem_list */

  /* add the size of the block info also to the size of the block */
  block->size = size + sizeof(block_info_t);
  block->prev = NULL;
  block->next = free_mem_list;

  if (block->next) {
    block->next->prev = block;
  }

  free_mem_list = block;
}

/**
 * Allocate a block of memory from free_mem_list. The allocated block does not
 * include the block info, neither does it reserve space for it. The called
 * must include the size of block info before calling this function.
 *
 * @param   size_t      The size of the block to allocate
 * @return  void *      The address of the allocated block
 */
static void *malloc_raw(size_t size)
{
  block_t *block = find_block(size);

  /* calculate the remaining size of the block after allocating the given sized
   * block */
  size_t remaining = block->size - size;

  if (!remaining) {
    /* remove this node from the list */

    if (block->prev) {
      block->prev->next = block->next;
    }

    if (block->next) {
      block->next->prev = block->prev;
    }

    /* return the address of this block */
    return block;
  }

  block->size = remaining;

  /* return the part at the end of this block */
  return (void *)block + remaining;
}

void *malloc(size_t size)
{
  /* allocate memory for the requested sized block + info about the block */
  block_info_t *alloc = malloc_raw(size + sizeof(block_info_t));

  /* store the info for the block */
  alloc->size = size;

  /* return the block after the info */
  return (void *)alloc + sizeof(block_info_t);
}

void *realloc(void *block, size_t size)
{
  free(block);

  return malloc(size);
}

void *calloc(size_t num_elements, size_t element_size)
{
  size_t size = num_elements * element_size;

  /* allocate memory for all the array elements */
  void *mem = malloc(size);

  /* set the memory to 0 */
  memset(mem, 0, size);

  return mem;
}

void free(void *block)
{
  block_info_t *info = block - sizeof(block_info_t);

  free_block(info);
}
