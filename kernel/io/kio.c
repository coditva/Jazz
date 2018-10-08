#include <io.h>
#include <stdarg.h>
#include "video/video.h"
#include "config.h"

#define MAX_BUFFER_SIZE 1024
static char buffer[MAX_BUFFER_SIZE];

int kprintf(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  video_write(buffer, ATTR_NORMAL);
}

int kputc(const char data)
{
  char str[2];
  str[0] = data;
  str[1] = '\0';
  video_write(str, ATTR_NORMAL);
}

#ifdef DEBUG
int err_attr[] = { 0x0c, 0x06, 0x03, 0x07 };

void keprintf(int error_level, const void *format, ...)
{
  va_list args;
  va_start(args, format);

  vsprintf(buffer, format, args);
  video_write(buffer, err_attr[error_level]);
#ifdef DEBUG_TO_SERIAL
  serial_write(DEBUG_SERIAL_PORT, buffer);
#endif
}
#endif
