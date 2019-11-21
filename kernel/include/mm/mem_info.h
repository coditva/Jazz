#ifndef MEM_INFO_H_W6L7HAGP
#define MEM_INFO_H_W6L7HAGP

#include <multiboot.h>
#include <types.h>

struct mem_info
{
  size_t num_pages;
  size_t base_address;
};

extern struct mem_info *memory_info;

#endif /* end of include guard: MEM_INFO_H_W6L7HAGP */
