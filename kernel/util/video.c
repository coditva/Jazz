#include "video.h"

#define VGA_BUFFER (char *)0xb8000      /* the VGA buffer memory */
#define SCREEN_SIZE 25 * 80 * 2         /* 25 rows, 80 columns, ascii+attr */

#define ATTR_BLANK      0x07
#define ATTR_HIGHLIGHT  0x02

static int position = 0;

void video_clear()
{
  char *video_memory = VGA_BUFFER;
  for (int i = 0; i < SCREEN_SIZE; i += 2) {
    video_memory[i] = ' ';            /* the character to print */
    video_memory[i + 1] = ATTR_BLANK; /* the character attribute */
  }
}

void video_print_str(const char *str)
{
  char *video_memory = VGA_BUFFER;
  for (int i = 0; str[i] != '\0'; i++) {
    video_print_char(str[i]);
  }
}

void video_print_char(const char ch)
{
  char *video_memory = VGA_BUFFER;
  video_memory[position++] = ch;
  video_memory[position++] = ATTR_HIGHLIGHT;
}
