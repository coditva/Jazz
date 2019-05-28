#ifndef TYPES_H_A3DIQNTC
#define TYPES_H_A3DIQNTC

#include <types.h>

/**
 * Struct for storing page directory entry information
 */
struct page_directory_entry {
  union {
    struct {
      uint32_t    present     :1;     /* valid entry */
      uint32_t    rw          :1;     /* 1 = read write, 0 = read only */
      uint32_t    is_user     :1;     /* 1 = user, 0 = supervisor only */
      uint32_t    write_thru  :1;     /* 1 = write-through, 0 = write-back */
      uint32_t    cache_d     :1;     /* 1 = cache disabled, 0 = cached */
      uint32_t    accessed    :1;     /* accessed since last refresh */
      uint32_t    _UNUSED_    :1;
      uint32_t    size        :1;     /* 1 = 4mb pages, 0 = 4kb pages */
      uint32_t    _FREE_      :4;     /* free for OS */
      uint32_t    address    :20;     /* address of the page_table entry left
                                         shifted by 12 bits (0x1000) */
    };
    uint32_t value;
  };
};

/**
 * Struct for storing page table entry information
 */
struct page_table_entry {
  union {
    struct {
      uint32_t    present     :1;     /* valid entry */
      uint32_t    rw          :1;     /* 1 = read write, 0 = read only */
      uint32_t    is_user     :1;     /* 1 = user, 0 = supervisor only */
      uint32_t    write_thru  :1;     /* 1 = write-through, 0 = write-back */
      uint32_t    cache_d     :1;     /* 1 = cache disabled, 0 = cached */
      uint32_t    accessed    :1;     /* accessed since last refresh */
      uint32_t    dirty       :1;     /* written since last refresh */
      uint32_t    _UNUSED_    :1;
      uint32_t    global      :1;     /* TODO */
      uint32_t    _FREE_      :3;     /* free for OS */
      uint32_t    address    :20;     /* physical address of the frame left
                                         shifted by 12 bits (0x1000) */
    };
    uint32_t value;
  };
};


/**
 * Struct for storing page information. This keeps track of the usage of each
 * physical page in the kernel.
 */
struct page {
  void      *address;
  void      *virtual_address;
};

#endif /* end of include guard: TYPES_H_A3DIQNTC */
