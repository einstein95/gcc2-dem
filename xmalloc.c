#include "xmalloc.h"

#include <stdlib.h>

void fatal PARAMS ((char *));

PTR
xmalloc (size)
  size_t size;
{
  register PTR value = (PTR) malloc (size);
  if (value == 0)
    fatal ("virtual memory exhausted");
  return value;
}

PTR
xrealloc (ptr, size)
  PTR ptr;
  size_t size;
{
  register PTR value = (PTR) realloc (ptr, size);
  if (value == 0)
    fatal ("virtual memory exhausted");
  return value;
}