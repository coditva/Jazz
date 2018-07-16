#include "util/video.h"

extern void kmain()
{
  const char *message = "Look Ma, no asm!";
  video_clear();
  video_print_str(message);
  return;
}
