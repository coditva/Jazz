#include "util/video.h"

extern void kmain()
{
  const char *message = "Look Ma, no asm!";
  kclear_screen();
  kputs(message);
  return;
}
