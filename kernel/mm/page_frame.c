#include <types.h>
#include <page_frame.h>
#include <multiboot.h>
#include <logger.h>

/* TODO: These don't belong here */
#define BITS_PER_BYTE   4
#define NUM_OF_A_PER_B(A, B)    (( (A + B) / (B + 1) ) + 1)

static void memset(const void *base_addr, int c, size_t size)
{
  for (int i = 0; i < 20; i++) {
    *((int *)base_addr + i) = c;
  }
}

#define BLOCK_SIZE  1024    /* 1kb */
#define FRAME_SIZE  4096    /* 4kb */

static uint32_t *frames_bitset = 0x0;
static int      num_frames = 0;
static int      num_frames_bitset_lines = 0;
static int      num_frames_bitset_frames = 0;

#define FRAMES_PER_BITSET_LINE      (BITS_PER_BYTE * sizeof(*frames_bitset))
#define BITSET_LINES_PER_FRAME      (FRAME_SIZE / sizeof(*frames_bitset))

#define PAGE_ADDR_TO_LINE(ADDR)     (ADDR / FRAMES_PER_BITSET_LINE)
#define PAGE_ADDR_TO_BIT(ADDR)      (0x1 << (ADDR % FRAMES_PER_BITSET_LINE))

#define PAGE_NUM_TO_ADDR_OFFSET(NUM)    (NUM * FRAME_SIZE)

static inline void page_frame_mark_free(uint64_t base_addr)
{
  frames_bitset[PAGE_ADDR_TO_LINE(base_addr)] |= PAGE_ADDR_TO_BIT(base_addr);
}

static inline void page_frame_mark_n_free(uint64_t base_addr, int num_pages)
{
  /* TODO: Optimize to mark bitset in one go */
  for (int i = 0; i < num_pages; i++) {
    page_frame_mark_free(base_addr + i);
  }
}

static inline void frames_bitset_init(void *base_addr)
{
  /* TODO: Figure out pointer size stuff. This is arch dependant */
  frames_bitset = base_addr;

  /* We assume that num_frames et al. are initialized before calling this */
  memset(frames_bitset, 0, num_frames_bitset_frames * FRAME_SIZE);
}

int page_frame_init(multiboot_info_t *multiboot_info)
{
  int bitset_initialized = 0;

  num_frames = (multiboot_info->mem_lower + multiboot_info->mem_upper) /
      (FRAME_SIZE / BLOCK_SIZE);
  num_frames_bitset_lines = NUM_OF_A_PER_B(num_frames, FRAMES_PER_BITSET_LINE);
  num_frames_bitset_frames =
      NUM_OF_A_PER_B(num_frames_bitset_lines, BITSET_LINES_PER_FRAME);

  klog(LOG_DEBUG, "Number of frames: %d\n", num_frames);
  klog(LOG_DEBUG, "Number of bitset lines: %d\nNumber of bitset pages: %d\n",
      num_frames_bitset_lines, num_frames_bitset_frames);

  /* mark all free pages */
  FOREACH_MEMORY_MAP(mmap, multiboot_info) {
    if (mmap->type == MULTIBOOT_MEM_TYPE_FREE) {
      if (!bitset_initialized) {
        frames_bitset_init((void *)(unsigned long)
            types_concat(mmap->base_addr_high, mmap->base_addr_low));

        /* reserve "num_frames_bitset_pages" pages for the frames_bitset */
        page_frame_mark_n_free(
            types_concat(mmap->base_addr_high, mmap->base_addr_low) +
                PAGE_NUM_TO_ADDR_OFFSET(num_frames_bitset_frames),
            types_concat(mmap->len_high, mmap->len_low) / FRAME_SIZE);
        bitset_initialized = 1;
      } else {
        page_frame_mark_n_free(
            types_concat(mmap->base_addr_high, mmap->base_addr_low),
            types_concat(mmap->len_high, mmap->len_low) / FRAME_SIZE);
      }
    }
  }
}

void page_frame_dump_map(void)
{
#ifdef DEBUG
  uint32_t *frame = frames_bitset;
  klog(LOG_DEBUG, "frame_bitset addr: 0x%x\n", frame);
  for (int j = 0; j < num_frames_bitset_frames * num_frames_bitset_lines; j++) {
    for (int k = 0; k < FRAMES_PER_BITSET_LINE; k++) {
      if (frame[j] & (0x1 << k)) { /* free */
        klog(LOG_DEBUG, ".");
      } else {
        klog(LOG_DEBUG, "#");
      }
    }
    klog(LOG_DEBUG, "%c", (j + 1) % 4 ? ' ' : '\n');
  }
#endif
}

uint32_t * page_frame_alloc(void)
{
}

int page_frame_free(uint32_t *page_addr)
{
}
