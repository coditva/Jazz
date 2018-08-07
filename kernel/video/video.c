#include <stdarg.h>
#include "video.h"
#include "util/types.h"
#include "util/portio.h"
#include "util/serial.h"

#include "config.h"

#define VGA_BUFFER (char *)0xb8000      /* the VGA buffer memory */
#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * 2)

#define ATTR_NORMAL     0x0f

#define _PUT_CHAR_AT(_CHAR, _POS) \
    video_memory[_POS] = _CHAR; \
    video_memory[_POS + 1] = attr;

#define _PUT_CHAR_ATTR_AT(_CHAR, _ATTR, _POS) \
    video_memory[_POS] = _CHAR; \
    video_memory[_POS + 1] = _ATTR;

static char *video_memory = VGA_BUFFER;
static int position = 0;
static int attr = ATTR_NORMAL;

#define get_attr          attr
#define set_attr(_ATTR) { attr = _ATTR; }
#define clear_attr      { attr = ATTR_NORMAL; }

void kclear_screen()
{
  for (int i = 0; i < SCREEN_SIZE; i += 2) {
    _PUT_CHAR_AT(' ', i);
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
  if (ch == 0) {
    return;
  } else if (ch == '\n') {
    kputnewline();
  } else if (ch == '\b') {
    position -= 2;
    _PUT_CHAR_AT(' ', position);
  } else {
    _PUT_CHAR_AT(ch, position);
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
  update_cursor();
}

#ifdef DEBUG
int err_attr[] = { 0x0c, 0x06, 0x03, 0x07 };

void eprintf(int error_level, const void *format, ...)
{
  int *args = (int *)&format;
  args++;

  set_attr(err_attr[error_level]);
  kprintf(format, args);
  clear_attr;
}
#endif

void update_cursor(void)
{
  int pos = position / 2;
	write_port(0x3D4, 0x0F);
	write_port(0x3D5, (uint8_t) (pos & 0xFF));
	write_port(0x3D4, 0x0E);
	write_port(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
