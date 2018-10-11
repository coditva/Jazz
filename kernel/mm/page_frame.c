#include <types.h>
#include <page_frame.h>
#include <multiboot.h>
#include <logger.h>
#include <string.h>

/* TODO: This doesn't belong here */
#define NUM_OF_A_PER_B(A, B)    ((A + B - 1) / B)

static char     *memory_base_addr;

static uint32_t *frames_bitset = NULL;

static int      num_frames = 0;
static int      num_frames_bitset_lines = 0;
static int      num_frames_bitset_frames = 0;

#define BLOCK_SIZE  1024    /* 1kb */
#define FRAME_SIZE  4096    /* 4kb */

#define FRAMES_PER_BITSET               (8 * sizeof(*frames_bitset))
#define BITSET_LINES_PER_FRAME          (FRAME_SIZE / sizeof(*frames_bitset))

#define FRAME_NUM_TO_PAGE_ADDR(NUM)     (NUM << 12)

#define FRAME_LINE_TO_PAGE_NUM(LINE)    (LINE << 5)

#define PAGE_ADDR_TO_FRAME_NUM(ADDR)    ((unsigned long)ADDR >> 12)
#define PAGE_ADDR_TO_LINE(ADDR) \
    (((unsigned long)ADDR >> 12) / FRAMES_PER_BITSET)
#define PAGE_ADDR_TO_BIT(ADDR) \
    (0x1 << (PAGE_ADDR_TO_FRAME_NUM(ADDR) % FRAMES_PER_BITSET))


static inline void page_frame_mark_free(void *base_addr)
{
  frames_bitset[PAGE_ADDR_TO_LINE(base_addr)] |= PAGE_ADDR_TO_BIT(base_addr);
}

static inline void page_frame_mark_n_free(void *base_addr, size_t len)
{
  /* TODO: Optimize to mark bitset in one go */
  char *addr = base_addr;
  char *end_addr = addr + len;
  while (addr < end_addr) {
    page_frame_mark_free(addr);
    addr += FRAME_SIZE;
  }
}

static inline void frames_bitset_init(void *base_addr)
{
  frames_bitset = base_addr;

  /* We assume that num_frames et al. are initialized before calling this */
  memset(frames_bitset, 0, num_frames_bitset_lines * sizeof(*frames_bitset));
}

int page_frame_init(multiboot_info_t *multiboot_info)
{
  int bitset_initialized = 0;

  memory_base_addr = 0x0;

  num_frames = (multiboot_info->mem_lower + multiboot_info->mem_upper) /
      (FRAME_SIZE / BLOCK_SIZE);
  num_frames_bitset_lines = NUM_OF_A_PER_B(num_frames, FRAMES_PER_BITSET);
  num_frames_bitset_frames =
      NUM_OF_A_PER_B(num_frames_bitset_lines, BITSET_LINES_PER_FRAME);

  /* mark all free pages */
  FOREACH_MEMORY_MAP(mmap, multiboot_info) {
    if (mmap->type == MULTIBOOT_MEM_TYPE_FREE) {
      if (!bitset_initialized) {
        frames_bitset_init((void *)(unsigned long)mmap->base_addr_low);

        /* reserve "num_frames_bitset_pages" pages for the frames_bitset */
        page_frame_mark_n_free((void *)(unsigned long)
            mmap->base_addr_low
                + FRAME_NUM_TO_PAGE_ADDR(num_frames_bitset_frames),
            mmap->len_low - FRAME_NUM_TO_PAGE_ADDR(num_frames_bitset_frames));
        bitset_initialized = 1;
      } else {
        page_frame_mark_n_free((void *)(unsigned long)mmap->base_addr_low,
            mmap->len_low);
      }
    }
  }
}

void page_frame_dump_map(void)
{
#ifdef DEBUG
  uint32_t *frame = frames_bitset;
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "PAGE FRAMES MEMORY DUMP START\n");
  klog(LOG_DEBUG, LOG_HRULE);
  klog(LOG_DEBUG, "Number of frames: %d\n", num_frames);
  klog(LOG_DEBUG, "Number of bitset lines: %d\n", num_frames_bitset_lines);
  klog(LOG_DEBUG, "Number of bitset pages: %d\n", num_frames_bitset_frames);
  klog(LOG_DEBUG, "Frames bitmap at 0x%x\n", frame);
  for (int j = 0; j < num_frames_bitset_lines; j++) {
    klog(LOG_DEBUG, "\n%x:", FRAME_NUM_TO_PAGE_ADDR(j * FRAMES_PER_BITSET));
    for (int k = 0; k < FRAMES_PER_BITSET; k++) {
      (k % 4) || klog(LOG_DEBUG, " ");
      if (frame[j] & (0x1 << k)) { /* free */
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
#endif
}

void * page_frame_alloc(void)
{
  static uint32_t current_frame_bitset_line = 0;
  static uint32_t current_frame_bitset_bit = 0;

  klog(LOG_DEBUG, "page_frame_alloc: Allocating frame ");

  /* go to next free frame line */
  while (frames_bitset[current_frame_bitset_line] == 0) {
    klog(LOG_DEBUG, "x");
    current_frame_bitset_bit = 0;
    current_frame_bitset_line++;
    if (current_frame_bitset_line == num_frames_bitset_lines) {
      /* wrap around */
      current_frame_bitset_line = 0;
    }
  }
  uint32_t *current_bitset_line = &frames_bitset[current_frame_bitset_line];

  /* go to next free frame bit */
  uint32_t current_bitset = 0x1 << current_frame_bitset_bit;
  while ((*current_bitset_line & current_bitset) == 0) {
    klog(LOG_DEBUG, "#");
    current_bitset = current_bitset << 1;
    current_frame_bitset_bit++;
    if (current_frame_bitset_bit == FRAMES_PER_BITSET) {
      /* wrap around */
      current_frame_bitset_bit = 0;
      current_bitset = 0x1;
    }
  }

  /* mark as used */
  *current_bitset_line ^= current_bitset;

  void *frame_address = (memory_base_addr + FRAME_NUM_TO_PAGE_ADDR(
      FRAME_LINE_TO_PAGE_NUM(current_frame_bitset_line)
      + current_frame_bitset_bit));
  klog(LOG_DEBUG, ". at 0x%x\n", frame_address);

  return frame_address;
}

void page_frame_free(void *page_addr)
{
  uint32_t line = PAGE_ADDR_TO_LINE(page_addr);
  uint32_t bit = PAGE_ADDR_TO_BIT(page_addr);

  if ((frames_bitset[line] & bit)) {
    klog(LOG_WARN,
        "page_frame_free: Attempt to free unallocated address at 0x%x\n",
        page_addr);
    return;
  }

  klog(LOG_DEBUG, "page_frame_free: Freeing frame at 0x%x\n", page_addr);
  page_frame_mark_free(page_addr);
}
