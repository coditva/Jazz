## Memory Management
This module contains implementations for various memory management routines

### `page_frame.c`
This is the lowest level module and is responsible for allocating physical
memory pages.

### `page_alloc.c`
This is a wrapper over page_frame. It takes care of allocating/de-allocating
chunks of pages from the physical memory. It wraps all allocated memory in the
`struct page` data structure.

### `paging.c`
This is a low level module for handling paging. It has routines to map/unmap
virtual addresses to physical addresses.
