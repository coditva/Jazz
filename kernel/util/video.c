#include <stdarg.h>
#include "video.h"

#define VGA_BUFFER (char *)0xb8000      /* the VGA buffer memory */
#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * 2)

#define ATTR_BLANK      0x07
#define ATTR_HIGHLIGHT  0x02

#define _PUT_CHAR_ATTR_AT(_CHAR, _ATTR, _POS) \
    video_memory[_POS] = _CHAR; \
    video_memory[_POS + 1] = _ATTR;

static char *video_memory = VGA_BUFFER;
static int position = 0;

void kclear_screen()
{
  for (int i = 0; i < SCREEN_SIZE; i += 2) {
    _PUT_CHAR_ATTR_AT(' ', ATTR_BLANK, i);
  }
}

void kputs(const char *str)
{
  for (int i = 0; str[i] != '\0'; i++) {
    kputc(str[i]);
  }
}

void kputc(const char ch)
{
  if (ch == '\n') {
    kputnewline();
  } else if (ch == '\b') {
    position -= 2;
    _PUT_CHAR_ATTR_AT(' ', ATTR_HIGHLIGHT, position);
  } else {
    _PUT_CHAR_ATTR_AT(ch, ATTR_HIGHLIGHT, position);
    position += 2;
  }
}

void kputbase(const int integer, const int base)
{
  int a = integer;
  char *out;
  int i;
  out[10] = '\0';
  for (i = 9; i > 0 && a != 0; i--) {
    out[i] = (a % base) + '0';
    a /= base;
  }
  kputs(out + i + 1);
}

void kputd(const int integer)
{
  kputbase(integer, 10);
}

void kputhex(const int integer)
{
  kputbase(integer, 16);
}

void kputnewline(void)
{
  int left = position % (2 * SCREEN_WIDTH);
  position += (2 * SCREEN_WIDTH) - left;
}

void kprintf(const void *format, ...)
{
  int *args;
  char *format_p = (char *)format;
  args = (int *)&format;
  args++;

  while (*format_p != '\0') {
    if (*format_p == '%') {
      format_p++;
      int x = *args++;
      if (*format_p == 'c') {
        kputc(x);
      } else if (*format_p == 's') {
        kputs((char *)x);
      } else if (*format_p == 'd') {
        kputd(x);
      } else if (*format_p == 'x') {
        kputhex(x);
      } else {
        /*error();*/
      }
    } else {
      kputc(*format_p);
    }
    format_p++;
  }
}
