#ifndef KIO_H_PI0IE1TW
#define KIO_H_PI0IE1TW

int     kputc       (const char data);
int     kprintf     (const char *format, ...);

#ifdef DEBUG
void    keprintf    (int error_level, const void *format, ...);
#endif


#endif /* end of include guard: KIO_H_PI0IE1TW */
