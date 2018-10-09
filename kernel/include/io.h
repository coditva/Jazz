#ifndef IO_H_F7KRHNJL
#define IO_H_F7KRHNJL

#include <stdarg.h>

int sputc      (char *buffer, const char data);
int sputd      (char *buffer, const int data);
int sputx      (char *buffer, const unsigned int data);
int sputs      (char *buffer, const char *data);
int sprintf    (char *buffer, const char *format, ...);

int vsprintf   (char *buffer, const char *format, va_list args);

#endif /* end of include guard: IO_H_F7KRHNJL */
