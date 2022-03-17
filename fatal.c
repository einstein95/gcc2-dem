#include <stdio.h>
#include <stdlib.h>

extern char *program_name;

void
fatal (str)
     char *str;
{
  fprintf (stderr, "%s: %s\n", program_name, str);
  exit (1);
}