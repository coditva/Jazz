#include <types.h>
#include <page_frame.h>
#include <multiboot.h>
#include <string.h>
#include <logger.h>

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
#define BLOCKS_PER_FRAME    4               /* each block is 1KB */

/* value in bitmap for used/free */
#define FRAME_FREE  0
#define FRAME_USED  -1


static inline void frame_mark_used(int frame_line, int frame_index) {
  frames_bitmap[frame_line] |= (unsigned int)FRAME_USED << frame_index;
}

static inline void frame_mark_free(int frame_line, int frame_index) {
  frames_bitmap[frame_line] &= FRAME_FREE << frame_index;
}

static inline uintptr_t frame_get_address(int frame_line, int frame_index) {
  int offset = (frame_line * FRAMES_PER_BITMAP) + frame_index;
  return memory_base_address + (offset * FRAME_SIZE);
}


void page_frame_init (void *base_address, multiboot_info_t *multiboot_info)
{
  klog_status_init("page_frame");

  /* calculate and allocate space for frames_bitmap */
  frames_bitmap = base_address;
  num_frames = (multiboot_info->mem_upper) / BLOCKS_PER_FRAME;
  frames_bitmap_size = num_frames / FRAMES_PER_BITMAP;
  memset(frames_bitmap, FRAME_FREE, frames_bitmap_size);

  /* start memory allocation after frames_bitmap. doing this ensures that there
   * is memory allocated for the bitmap. */
  frames_bitmap_pages =
      (frames_bitmap_size + FRAME_SIZE_BYTES - 1) / FRAME_SIZE_BYTES;
  memory_base_address =
      ((uintptr_t)base_address + (FRAME_SIZE * frames_bitmap_pages));

  /* mark all free pages */
  FOREACH_MEMORY_MAP(mmap, multiboot_info) {
    if (mmap->type == MULTIBOOT_MEM_TYPE_FREE) {
      /* TODO: mark the whole memory at one go with memset */
      uintptr_t address = mmap->base_addr_low;
      while (address < mmap->base_addr_high || address < memory_base_address) {
        frame_mark_free(address / FRAMES_PER_BITMAP_BYTES,
            address % FRAMES_PER_BITMAP_BYTES);
        address += FRAME_SIZE_BYTES;
      }
    }
  }

  klog_status_ok("page_frame");
}

void * page_frame_alloc (void)
{
  static uint32_t current_bitmap_line = 0;
  static uint32_t current_bitmap_index = 0;

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
    klog(LOG_DEBUG, "#");
    current_bitmap_index += 1;
    if (current_bitmap_index == FRAMES_PER_BITMAP_BYTES) {  /* wrap around */
      current_bitmap_index = 0;
    }

    current_bitmap = (FRAME_FREE << current_bitmap_index) + 1;
  }
  klog(LOG_DEBUG, ".\n");

  frame_mark_used(current_bitmap_line, current_bitmap_index);
  return (void *)frame_get_address(current_bitmap_line, current_bitmap_index);
}

void page_frame_free (void *page_addr)
{
  uint32_t line, index;
  line = ((uintptr_t)page_addr - memory_base_address) / FRAMES_PER_BITMAP;
  index = ((uintptr_t)page_addr - memory_base_address) % FRAMES_PER_BITMAP;
  frame_mark_free(line, index);
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
  klog(LOG_DEBUG, "Frames bitmap at address   : 0x%x\n", &frames_bitmap);
  klog(LOG_DEBUG, "Base address for allocation: 0x%x\n", memory_base_address);

  for (uintptr_t j = 0; j < frames_bitmap_size + 1; j++) {
    klog(LOG_DEBUG, "\n0x%x:", frame_get_address(j, 0));

    for (uintptr_t k = 0; k < FRAMES_PER_BITMAP; k++) {
      if (k % 4 == 0)  /* add a space after every four marks */
        klog(LOG_DEBUG, " ");

      if ((frames_bitmap[j] | (FRAME_FREE << k)) == 0) {  /* free */
        klog(LOG_DEBUG, ".");
      } else {
        klog(LOG_DEBUG, "#");
      }
    }
  }

  klog(LOG_DEBUG, "\n");
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "PAGE FRAMES MEMORY DUMP END\n");
  klog(LOG_DEBUG, LOG_HRULE);
}
#endif
