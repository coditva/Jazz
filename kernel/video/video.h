#ifndef VIDEO_H_SIVRHMXN
#define VIDEO_H_SIVRHMXN

#include "config.h"

void kclear_screen();
void kputc(const char ch);
void kputs(const char *str);
void kputd(const int integer);
void kputhex(const int integer);
void kputnewline(void);
void kprintf(const void *format, ...);
void update_cursor(void);


#ifdef DEBUG
void eprintf(int error_level, const void *format, ...);
#else
# define eprintf(ERROR_LEVEL, FORMAT, ...)
#endif

#endif /* end of include guard: VIDEO_H_SIVRHMXN */
