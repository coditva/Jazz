#include <assert.h>
#include <logger.h>
#include <mm/mem_info.h>
#include <mm/page_frame.h>
#include <multiboot.h>
#include <string.h>
#include <types.h>

/* struct to store memory info */
struct mem_info *memory_info = NULL;

/* bitmap to store the state of the memory pages */
static uint32_t     *frames_bitmap = NULL;

/* number of frames in each bitmap */
#define FRAMES_PER_BITMAP       32
#define FRAMES_PER_BITMAP_BYTES 4

/* base address of memory from which pages are allocated */
static uintptr_t    memory_base_address = 0;

/* total number of frames available */
static uint32_t     num_frames = 0;

/* number of lines in the bitmap */
static uint32_t     frames_bitmap_size;

/* number of pages allocated to bitmap */
static uint32_t     frames_bitmap_pages;


/* size of each frame that is allocated in bytes */
#define FRAME_SIZE          4096            /* 4KB */
#define FRAME_SIZE_BYTES    512             /* 4096/8 */
#define BLOCK_SIZE          1024
#define BLOCKS_PER_FRAME    4               /* each block is 1KB */

/* value in bitmap for used/free */
#define FRAME_FREE  0
#define FRAME_USED  (-1)


/* keeping track of where we are on bitmap while allocation/freeing */
static uint32_t current_bitmap_line = 0;
static uint32_t current_bitmap_index = 0;


static inline void frame_mark_used(uint64_t frame_line, int frame_index) {
  frames_bitmap[frame_line] |= 1 << frame_index;
}

static inline void frame_mark_free(uint64_t frame_line, int frame_index) {
  frames_bitmap[frame_line] &= ~(1 << frame_index);
}

static inline int frame_is_used(uint64_t frame_line, int frame_index) {
  return frames_bitmap[frame_line] & 1 << frame_index;
}

static inline uintptr_t frame_get_address(int frame_line, int frame_index) {
  int offset = (frame_line * FRAMES_PER_BITMAP) + frame_index;
  return memory_base_address + (offset * FRAME_SIZE);
}


void page_frame_init (void *base_address, multiboot_info_t *multiboot_info)
{
  klog_status_init("page_frame");

  /* allocate memory for mem_info */
  memory_info = base_address;
  base_address += sizeof(struct mem_info);

  /* align address to frame size */
  base_address = (void *)((uintptr_t)base_address & ~(FRAME_SIZE - 1));
  base_address += FRAME_SIZE;
  assert(((uintptr_t)base_address & 0xfff) == 0);

  /* calculate and allocate space for frames_bitmap */
  frames_bitmap = base_address;
  num_frames = (multiboot_info->mem_upper) / BLOCKS_PER_FRAME;
  frames_bitmap_size = num_frames / FRAMES_PER_BITMAP;
  memset(frames_bitmap, FRAME_USED, frames_bitmap_size);

  /* start memory allocation after frames_bitmap. doing this ensures that there
   * is memory allocated for the bitmap. */
  frames_bitmap_pages =
      (frames_bitmap_size + FRAME_SIZE_BYTES - 1) / FRAME_SIZE_BYTES;
  memory_base_address =
      ((uintptr_t)base_address + (FRAME_SIZE * frames_bitmap_pages));


  /* mark all free pages */
  FOREACH_MEMORY_MAP(mmap, multiboot_info) {
    uintptr_t memory_lower_range = memory_base_address;
    uintptr_t memory_upper_range = multiboot_info->mem_upper * BLOCK_SIZE;
    uintptr_t mmap_limit = mmap->base_addr_low + mmap->len_low;

    /* ensure they are inside our bitmap range i.e. from
     * `memory_base_address` to `multiboot_info->mem_upper` */
    if (mmap->type == MULTIBOOT_MEM_TYPE_FREE
        && !(memory_lower_range < mmap->base_addr_low
            && memory_upper_range < mmap_limit)
        && !(memory_lower_range > mmap->base_addr_low
            && memory_upper_range > mmap_limit)
        ) {

      /* change lower and upper ranges a/c to mmap */
      if (memory_lower_range < mmap->base_addr_low) {
        memory_lower_range = mmap->base_addr_low;
      }
      if (memory_upper_range > mmap_limit) {
        memory_upper_range = mmap_limit;
      }

      /* scale ranges to frame_bitmap */
      memory_lower_range =
        (memory_lower_range - memory_base_address) / FRAME_SIZE;
      memory_upper_range =
        (memory_upper_range - memory_base_address) / FRAME_SIZE;

      /* mark the memory as free.
       * TODO: mark the whole memory at one go with memset */
      while (memory_lower_range < memory_upper_range) {
        frame_mark_free(memory_lower_range / FRAMES_PER_BITMAP,
            memory_lower_range % FRAMES_PER_BITMAP);
        memory_lower_range += 1;
      }
    }
  }

  memory_info->base_address = (size_t)base_address;
  memory_info->num_pages = num_frames;

  klog_status_ok("page_frame");
}

inline void * page_frame_alloc(void)
{
  return page_frame_n_alloc(1);
}

void * page_frame_n_alloc (int number_of_pages)
{
  klog(LOG_DEBUG, "page_frame_alloc: Allocating frame ");

  /* go to next free frame line */
  while (frames_bitmap[current_bitmap_line] == (unsigned)FRAME_USED) {/* miss */
    klog(LOG_DEBUG, "x");
    current_bitmap_line += 1;

    if (current_bitmap_line == frames_bitmap_size) {  /* wrap around */
      current_bitmap_line = 0;
    }
  }

  /* go to next free frame bit */
  uint32_t current_bitmap = (FRAME_FREE << current_bitmap_index) + 1;
  while ((frames_bitmap[current_bitmap_line] | current_bitmap) == 0) {/* miss */
    klog(LOG_DEBUG, "x");
    current_bitmap_index += 1;
    if (current_bitmap_index == FRAMES_PER_BITMAP) {  /* wrap around */
      current_bitmap_index = 0;
    }

    current_bitmap = (FRAME_FREE << current_bitmap_index) + 1;
  }

  /* allocate required pages */
  int allocated_pages = 0;
  int32_t start_line = 0;
  int32_t start_index = 0;
  while (allocated_pages < number_of_pages) {
    current_bitmap = (FRAME_FREE << current_bitmap_index) + 1;
    if ((frames_bitmap[current_bitmap_line] | current_bitmap) == 0) {/* miss */
      allocated_pages = 0;
      klog(LOG_DEBUG, "#");
    } else {
      allocated_pages += 1;
    }

    if (allocated_pages == 1) {
      start_line = current_bitmap_line;
      start_index = current_bitmap_index;
    }

    current_bitmap_index += 1;
    if (current_bitmap_index == FRAMES_PER_BITMAP) { /* go to next line */
      current_bitmap_line += 1;
      current_bitmap_index = 0;
    }
  }

  klog(LOG_DEBUG, ".\n");

  int line = start_line;
  int index = start_index;
  for (int i = 0; i < number_of_pages; ++i) {
    frame_mark_used(line, index);
    index += 1;
    if (index == FRAMES_PER_BITMAP) {
      index = 0;
      line += 1;
    }
  }

  uintptr_t address = frame_get_address(start_line, start_index);
  assert(((uintptr_t)address & 0xfff) == 0);
  return (void *)address;
}

inline void page_frame_free (void *page_addr) {
  page_frame_n_free(page_addr, 1);
}

void page_frame_n_free (void *page_addr, int number_of_pages)
{
  assert(((uintptr_t)page_addr & 0xfff) == 0);  /* page_addr is aligned */

  uintptr_t addr = ((uintptr_t)page_addr - memory_base_address) / FRAME_SIZE;
  uint32_t line = addr / FRAMES_PER_BITMAP;
  uint32_t index = addr % FRAMES_PER_BITMAP;

  current_bitmap_line = line;
  current_bitmap_index = index;

  for (int i = 0; i < number_of_pages; ++i) {
    assert(frame_is_used(line, index));
    frame_mark_free(line, index);

    index += 1;
    if (index == FRAMES_PER_BITMAP) {
      index = 0;
      line += 1;
    }
  }
}

#ifdef DEBUG
void page_frame_dump_map(void)
{
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "PAGE FRAMES MEMORY DUMP START\n");
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "Number of frames           : %d\n", num_frames);
  klog(LOG_DEBUG, "Number of bitmap lines     : %d\n", frames_bitmap_size);
  klog(LOG_DEBUG, "Number of bitmap pages     : %d\n", frames_bitmap_pages);
  klog(LOG_DEBUG, "Frames bitmap at address   : 0x%x\n", frames_bitmap);
  klog(LOG_DEBUG, "Base address for allocation: 0x%x\n", memory_base_address);

  for (uintptr_t j = 0; j < frames_bitmap_size; j++) {
    klog(LOG_DEBUG, "\n0x%x:", frame_get_address(j, 0));

    for (uintptr_t k = 0; k < FRAMES_PER_BITMAP; k++) {
      if (k % 4 == 0) {  /* add a space after every four marks */
        klog(LOG_DEBUG, " ");
      }

      if ((frames_bitmap[j] & (1 << k))) {  /* used */
        klog(LOG_DEBUG, "#");
      } else {
        klog(LOG_DEBUG, ".");
      }
    }
  }

  klog(LOG_DEBUG, "\n");
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "PAGE FRAMES MEMORY DUMP END\n");
  klog(LOG_DEBUG, LOG_HRULE);
}
#endif
