#include <types.h>
#include <page_frame.h>
#include <multiboot.h>
#include <string.h>
#include <logger.h>

/* bitmap to store the state of the memory pages */
static uint32_t     *frames_bitmap = NULL;

/* base address of memory from which pages are allocated */
static uintptr_t    memory_base_address = 0;

/* total number of frames available */
static uint32_t     num_frames = 0;

/* number of lines in the bitmap */
static uint32_t     frames_bitmap_size;

/* number of pages allocated to bitmap */
static uint32_t     frames_bitmap_pages;


/* size of each frame that is allocated in bytes */
#define FRAME_SIZE  (4096 / 8)  /* 4KB */
#define BITMAP_SIZE (8 * sizeof(*frames_bitmap))

#define FRAME_SIZE_KB 4

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
  int offset = (frame_line * sizeof(frames_bitmap) * 8) + frame_index;
  return memory_base_address + offset;
}

static inline void memory_mark_free(uintptr_t low_address,
    uintptr_t high_address)
{
  uintptr_t address = low_address - memory_base_address;
  uintptr_t end_address = high_address - memory_base_address;

  while (address < end_address) {
    int frame_line = address / sizeof(*frames_bitmap);
    int frame_index = address % sizeof(*frames_bitmap);
    frame_mark_free(frame_line, frame_index);
    address += FRAME_SIZE;
  }
}


void page_frame_init(uintptr_t base_address, multiboot_info_t *multiboot_info)
{
  klog_status_init("page_frame");

  /* calculate and allocate space for frames_bitmap */
  frames_bitmap = (void *)base_address;
  num_frames = (multiboot_info->mem_upper + FRAME_SIZE_KB - 1) / FRAME_SIZE_KB;
  frames_bitmap_size = (num_frames + BITMAP_SIZE - 1) / BITMAP_SIZE;
  memset(frames_bitmap, FRAME_USED,
      frames_bitmap_size * sizeof(*frames_bitmap));

  /* start memory allocation after frames_bitmap. doing this ensures that there
   * is memory allocated for the bitmap. */
  frames_bitmap_pages = (frames_bitmap_size + FRAME_SIZE - 1) / FRAME_SIZE;
  memory_base_address =
    base_address + (FRAME_SIZE * frames_bitmap_pages);

  /* mark all free pages */
  FOREACH_MEMORY_MAP(mmap, multiboot_info) {
    if (mmap->type == MULTIBOOT_MEM_TYPE_FREE) {
      klog(LOG_DEBUG, "size: %x\n", sizeof(uintptr_t));
      klog(LOG_DEBUG, "size: %x\n", sizeof(uint32_t));
      klog(LOG_DEBUG, "size: %x\n", sizeof(uint64_t));
      uintptr_t address = 0// ((uintptr_t)mmap->base_addr_high << 32)
          + mmap->base_addr_low;
      /*klog(LOG_DEBUG, "Free memory at 0x%x\n", address);*/
      memory_mark_free(address, address + mmap->len_low);
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
  while (frames_bitmap[current_bitmap_line] == (unsigned)-1) {  /* miss */
    klog(LOG_DEBUG, "x");
    current_bitmap_line += 1;

    if (current_bitmap_line == frames_bitmap_size) {  /* wrap around */
      current_bitmap_line = 0;
    }
  }

  /* go to next free frame bit */
  uint32_t current_bitmap = (FRAME_FREE << current_bitmap_index) + 1;
  while ((frames_bitmap[current_bitmap_line] | current_bitmap) == 0) {  /* miss */
    klog(LOG_DEBUG, "#");
    current_bitmap_index += 1;
    if (current_bitmap_index == sizeof(*frames_bitmap)) {  /* wrap around */
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
  line = ((uintptr_t)page_addr - memory_base_address) / (sizeof(*frames_bitmap) * 8);
  index = ((uintptr_t)page_addr - memory_base_address) % (sizeof(*frames_bitmap) * 8);
  frame_mark_free(line, index);
}

#ifdef DEBUG
void page_frame_dump_map(void)
{
  uint32_t *frame = frames_bitmap;
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "PAGE FRAMES MEMORY DUMP START\n");
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "Number of frames           : %d\n", num_frames);
  klog(LOG_DEBUG, "Number of bitmap lines     : %d\n", frames_bitmap_size);
  klog(LOG_DEBUG, "Number of bitmap pages     : %d\n", frames_bitmap_pages);
  klog(LOG_DEBUG, "Frames bitmap at address   : 0x%x\n", frame);
  klog(LOG_DEBUG, "Base address for allocation: 0x%x\n", memory_base_address);

  for (unsigned int j = 0; j < frames_bitmap_size; j++) {
    klog(LOG_DEBUG, "\n0x%x:",
        memory_base_address + (j * sizeof(*frames_bitmap)));

    for (unsigned int k = 0; k < 8 * sizeof(*frames_bitmap); k++) {
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
