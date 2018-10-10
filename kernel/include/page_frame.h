#ifndef PAGE_FRAME_ALLOC_H_S1KP2ELH
#define PAGE_FRAME_ALLOC_H_S1KP2ELH

#include <types.h>
#include <multiboot.h>

int         page_frame_init     (multiboot_info_t *multiboot_info);
uint32_t *  page_frame_alloc    (void);
int         page_frame_free     (uint32_t *page_addr);
void        page_frame_dump_map (void);

#endif /* end of include guard: PAGE_FRAME_ALLOC_H_S1KP2ELH */
