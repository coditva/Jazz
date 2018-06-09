
#define VGA_BUFFER (char *)0xb8000      /* the VGA buffer memory */
#define SCREEN_SIZE 25 * 80             /* 25 rows, 80 columns */

#define ATTR_BLANK      0x07
#define ATTR_HIGHLIGHT  0x02

extern void kmain()
{
  char *video_memory = VGA_BUFFER;
  const char *message = "Look Ma, no asm!";
  unsigned i = 0, j = 0;

  /* clear the screen */
  while (j < SCREEN_SIZE * 2) {
    video_memory[j] = ' ';            /* the character to print */
    video_memory[j + 1] = ATTR_BLANK; /* the character attribute */
    j += 2;
  }

  /* print the "message" */
  for (i = 0, j = 0; message[i] != '\0'; i++, j += 2) {
    video_memory[j] = message[i];
    video_memory[j + 1] = ATTR_HIGHLIGHT;
  }

  return;
}
