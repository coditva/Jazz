#include "util/video.h"

extern void kmain()
{
  /*const char *message = "Look Ma, no asm!";*/
  kclear_screen();
  /*kputs(message);*/
  kprintf("this : %c : %s : %d", 'c', "a string", 123456789);
  return;
}
