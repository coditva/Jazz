#include <string.h>

int strcmp(const char* str1, const char* str2)
{
  int index = 0;
  while (str1[index] != '\0' && str2[index] != '\0') {
    if (str1[index] < str2[index]) {
      return -1;
    }
    if (str1[index] > str2[index]) {
      return 1;
    }
    index += 1;
  }

  if (str1[index] != '\0') {
    return -1;
  }
  if (str2[index] != '\0') {
    return 1;
  }

  return 0;
}
