#include <io.h>
#include <stdarg.h>

#define K_INTSTRING "0123456789abcdef"
#define K_INTWIDTH  36

int sputc(char *buffer, const char data)
{
  *buffer = data;
  return 1;
}

int sputs(char *buffer, const char *data)
{
  int i;
  for (i = 0; data[i] != '\0'; i++) {
    sputc(buffer++, data[i]);
  }
  buffer = '\0';
  return i;
}

static int sputi(char *buffer, const unsigned int data, const int radix)
{
  unsigned int a = data;
  char out[K_INTWIDTH];
  int i;
  out[K_INTWIDTH] = '\0';

  for (i = K_INTWIDTH - 1; i >= 0 && a != 0; i--) {
    out[i] = K_INTSTRING[a % radix];
    a /= radix;
  }
  if (i == K_INTWIDTH - 1) {  /* nothing printed */
    out[i] = '0';
    i--;
  }
  return sputs(buffer, out + i + 1) - 1;
}

inline __attribute__((always_inline))
int sputd(char *buffer, const int data)
{
  return sputi(buffer, data, 10);
}

inline __attribute__((always_inline))
int sputx(char *buffer, const unsigned int data)
{
  return sputi(buffer, data, 16);
}

int sprintf(char *buffer, const char *format, ...)
{
  /* TODO: Don't use va_list! */
  va_list args;
  va_start(args, format);
  return vsprintf(buffer, format, args);
}

int vsprintf(char *buffer, const char *format, va_list args)
{
  char *format_p = (char *)format;
  int size = 0;
  int retsize = 0;

  while (*format_p != '\0') {
    if (*format_p == '%') {
      format_p++;
      switch (*format_p) {
        case 'c':
          retsize = sputc(buffer, va_arg(args, char));
          break;

        case 's':
          retsize = sputs(buffer, va_arg(args, char *));
          break;

        case 'd':
          retsize = sputd(buffer, va_arg(args, int));
          break;

        case 'x':
          retsize = sputx(buffer, va_arg(args, unsigned int));
          break;

        default:
          retsize = 0;
          /*error();*/
      }
    } else {
      retsize = sputc(buffer, *format_p);
    }

    format_p++;
    size += retsize;
    buffer += retsize;
  }

  *buffer = '\0';
  return size + 1;
}
