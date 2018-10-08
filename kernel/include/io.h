#ifndef IO_H_F7KRHNJL
#define IO_H_F7KRHNJL

int vputc      (char *buffer, const char data);
int vputd      (char *buffer, const int data);
int vputx      (char *buffer, const int data);
int vputs      (char *buffer, const char *data);
int vsprintf   (char *buffer, const char *format, ...);

#endif /* end of include guard: IO_H_F7KRHNJL */
