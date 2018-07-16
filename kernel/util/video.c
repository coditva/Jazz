#include "video.h"

#define VGA_BUFFER (char *)0xb8000      /* the VGA buffer memory */
#define SCREEN_SIZE 25 * 80 * 2         /* 25 rows, 80 columns, ascii+attr */

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
  _PUT_CHAR_ATTR_AT(ch, ATTR_HIGHLIGHT, position);
  position += 2;
}
