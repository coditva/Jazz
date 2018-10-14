#include <io.h>
#include <kio.h>
#include <stdarg.h>
#include "io/video/video.h"
#include "io/serial/serial.h"
#include "config.h"

#define MAX_BUFFER_SIZE 1024
static char buffer[MAX_BUFFER_SIZE];

int kprintf(const char *format, ...)
{
  int retval = 0;
  va_list args;

  va_start(args, format);
  retval = kvprintf(format, args);
  va_end(args);

  return retval;
}

int kvprintf(const char *format, va_list args)
{
  int retval = 0;
  retval = vsprintf(buffer, format, args);
  video_write(buffer, ATTR_NORMAL);
  return retval;
}

int kputc(const char data)
{
  char str[2];
  str[0] = data;
  str[1] = '\0';
  video_write(str, ATTR_NORMAL);
  return 1;
}

#ifdef DEBUG
int err_attr[] = { 0x0c, 0x06, 0x03, 0x07 };

int keprintf(int error_level, const void *format, ...)
{
  int retval = 0;
  va_list args;

  va_start(args, format);
  retval = kveprintf(error_level, format, args);
  va_end(args);

  return retval;
}

int kveprintf(int error_level, const void *format, va_list args)
{
  int retval = vsprintf(buffer, format, args);
# ifdef DEBUG_TO_SERIAL
  serial_write(DEBUG_SERIAL_PORT, buffer);
# else
  video_write(buffer, err_attr[error_level]);
# endif
  return retval;
}
#endif
