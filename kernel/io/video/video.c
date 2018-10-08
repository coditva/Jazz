#include <stdarg.h>
#include <types.h>
#include <io.h>
#include "io/video/video.h"
#include "io/portio/portio.h"

#define VGA_BUFFER      ((char *)0xb8000)    /* the VGA buffer memory */

#define SCREEN_HEIGHT   25
#define SCREEN_WIDTH    80
#define SCREEN_SIZE     (SCREEN_WIDTH * SCREEN_HEIGHT * 2)

static int position = 0;

static void update_cursor(void)
{
  int pos = position / 2;
  write_port(0x3D4, 0x0F);
  write_port(0x3D5, (uint8_t) (pos & 0xFF));
  write_port(0x3D4, 0x0E);
  write_port(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void video_write(const char *data, const int attr)
{
  while(*data != '\0') {
    switch (*data) {
      case '\n':
        position += (2 * SCREEN_WIDTH) - (position % (2 * SCREEN_WIDTH));
        break;

      case '\b':
        position -= 2;
        VGA_BUFFER[position] = ' ';
        VGA_BUFFER[position + 1] = attr;
        break;

      default:
        VGA_BUFFER[position] = *data;
        VGA_BUFFER[position + 1] = attr;
        position += 2;
    }
    data++;
  }
  update_cursor();
}

void video_clear()
{
  for (int i = 0; i < SCREEN_SIZE; i += 2) {
    VGA_BUFFER[i] = ' ';
    VGA_BUFFER[i + 1] = ATTR_NORMAL;
  }
}
