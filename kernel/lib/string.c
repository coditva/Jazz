#include <string.h>
#include <types.h>

void *memset(void *base_address, uint16_t character, size_t size)
{
  char *s_p = base_address;
  while (size--) {
    *s_p++ = character;
  }
  return base_address;
}
