#ifndef PAGE_FRAME_ALLOC_H_S1KP2ELH
#define PAGE_FRAME_ALLOC_H_S1KP2ELH

#include <types.h>
#include <multiboot.h>

/**
 * Initializes the memory frames bitmap, which will be used later by alloc/free
 * functions. It also allocates some pages to store the said bitmap.
 *
 * @param uintptr_t         base address where kernel ends
 * @param multiboot_into_t  multiboot info
 */
void        page_frame_init     (uintptr_t base_address,
                                 multiboot_info_t *multiboot_info);

/**
 * Allocate a page from memory and return it's address.
 *
 * @return void *           Address of the allocated page
 */
void *      page_frame_alloc    (void);

/**
 * Free an allocated page from memory.
 *
 * @param void *            Address of an allocated page
 */
void        page_frame_free     (void *page_addr);


# ifdef DEBUG
/**
 * Dump the map of page frame on the debug log
 */
void        page_frame_dump_map (void);
# endif

#endif /* end of include guard: PAGE_FRAME_ALLOC_H_S1KP2ELH */
