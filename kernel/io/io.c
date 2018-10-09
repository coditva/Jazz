#include <io.h>
#include <stdarg.h>

#define K_INTSTRING "0123456789abcdef"
#define K_INTWIDTH  36

#define _ITOA_REV(BUFFER, DATA, RADIX, MIN_LEN, MAX_LEN, SIZE) { \
  BUFFER[0] = '\0'; \
  int i = 1; \
  for(; i < MAX_LEN && DATA != 0; i++) { \
    BUFFER[i] = K_INTSTRING[DATA % RADIX]; \
    DATA /= RADIX; \
  } \
  for (; i < MIN_LEN + 1; i++) { \
    BUFFER[i] = '0'; \
  } \
  SIZE = i; \
}

#define _STR_NREV(BUFFER_OUT, BUFFER_IN, N) { \
  for (int i = 0; i < N; i++) { \
    BUFFER_OUT[i] = BUFFER_IN[N - i - 1]; \
  } \
}

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

int sputd(char *buffer, int data)
{
  int size = 0;
  char out[K_INTWIDTH];
  _ITOA_REV(out, data, 10, 1, K_INTWIDTH, size);
  _STR_NREV(buffer, out, size);
  return size - 1;
}

int sputx(char *buffer, unsigned int data)
{
  int size = 0;
  char out[K_INTWIDTH];
  _ITOA_REV(out, data, 16, 1, K_INTWIDTH, size);
  _STR_NREV(buffer, out, size);
  return size - 1;
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
