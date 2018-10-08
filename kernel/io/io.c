#include <io.h>
#include <stdarg.h>

#define K_INTSTRING "0123456789abcdef"
#define K_INTWIDTH  36

int vputc(char *buffer, const char data)
{
  *buffer = data;
  return 1;
}

int vputs(char *buffer, const char *data)
{
  int i;
  for (i = 0; data[i] != '\0'; i++) {
    vputc(buffer++, data[i]);
  }
  buffer = '\0';
  return i;
}

int vputi(char *buffer, const int data, const int radix)
{
  int a = data;
  char out[K_INTWIDTH];
  int i;
  out[K_INTWIDTH] = '\0';

  for (i = K_INTWIDTH - 1; i >= 0 && a != 0; i--) {
    out[i] = K_INTSTRING[a % radix];
    a /= radix;
  }
  return vputs(buffer, out + i + 1) - 1;
}

inline __attribute__((always_inline))
int vputd(char *buffer, const int data)
{
  return vputi(buffer, data, 10);
}

inline __attribute__((always_inline))
int vputx(char *buffer, const int data)
{
  return vputi(buffer, data, 16);
}

int vsprintf(char *buffer, const char *format, ...)
{
  char *format_p = (char *)format;
  int size = 0;
  int retsize = 0;
  va_list args;
  va_start(args, format);

  while (*format_p != '\0') {
    if (*format_p == '%') {

      switch (*++format_p) {
        case 'c':
          retsize = vputc(buffer, va_arg(args, char));
          break;

        case 's':
          retsize = vputs(buffer, va_arg(args, char *));
          break;

        case 'd':
          retsize = vputd(buffer, va_arg(args, int));
          break;

        case 'x':
          retsize = vputx(buffer, va_arg(args, int));
          break;
/*
 *      default:
 *        error();
 */
      }
    } else {
      retsize = vputc(buffer, *format_p);
    }

    format_p++;
    size += retsize;
    buffer += retsize;
  }

  *buffer++ = '\0';
  return size + 1;
}
