#ifndef KIO_H_PI0IE1TW
#define KIO_H_PI0IE1TW

#include <stdarg.h>

int     kputc       (char data);

int     kprintf     (const char *format, ...);
int     keprintf    (int error_level, const void *format, ...);

int     kvprintf    (const char *format, va_list args);
int     kveprintf   (int error_level, const void *format, va_list args);

#endif /* end of include guard: KIO_H_PI0IE1TW */
