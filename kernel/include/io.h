#ifndef IO_H_F7KRHNJL
#define IO_H_F7KRHNJL

#include <stdarg.h>

int sputc(char *buffer, char data);
int sputd(char *buffer, int data);
int sputx(char *buffer, unsigned int data);
int sputs(char *buffer, const char *data);
int sprintf(char *buffer, const char *format, ...);

int vsprintf(char *buffer, const char *format, const va_list args);

#endif /* end of include guard: IO_H_F7KRHNJL */
