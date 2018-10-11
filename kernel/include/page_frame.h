#ifndef PAGE_FRAME_ALLOC_H_S1KP2ELH
#define PAGE_FRAME_ALLOC_H_S1KP2ELH

#include <types.h>
#include <multiboot.h>

int         page_frame_init     (multiboot_info_t *multiboot_info);
void *      page_frame_alloc    (void);
void        page_frame_free     (void *page_addr);
void        page_frame_dump_map (void);

#endif /* end of include guard: PAGE_FRAME_ALLOC_H_S1KP2ELH */
