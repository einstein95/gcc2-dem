#if !defined(XMALLOC_H)
#define XMALLOC_H

#include "ansidecl.h"
#include <stddef.h>

PTR xmalloc PARAMS((size_t));
PTR xrealloc PARAMS((PTR, size_t));

#endif