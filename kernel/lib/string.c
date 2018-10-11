#include <string.h>
#include <types.h>

void * memset(void *s, int c, size_t count)
{
  char *s_p = s;
  while (count--) {
    *s_p++ = c;
  }
  return s;
}
