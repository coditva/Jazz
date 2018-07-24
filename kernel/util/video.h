#ifndef VIDEO_H_SIVRHMXN
#define VIDEO_H_SIVRHMXN

void kclear_screen();
void kputc(const char ch);
void kputs(const char *str);
void kputd(const int integer);
void kputhex(const int integer);
void kputnewline(void);
void kprintf(const void *format, ...);

#endif /* end of include guard: VIDEO_H_SIVRHMXN */
