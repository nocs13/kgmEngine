#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "kgmLog.h"

void* kgm_alloc(size_t size)
{
  if (size < 1)
    return NULL;

  void* p = ::malloc(size);

  //void* p = calloc(size, 1);

#ifdef DEBUGXXX
  printf("Alloc memory: %p, size: %ld.\n", p, size);
#endif

  return p;
}

void* kgm_realloc(void *p, size_t size)
{
  void* pr = ::realloc(p, size);

  return pr;
}

void kgm_free(void* p)
{
#ifdef DEBUGXXX
  printf("Free memory: %p.\n", p);
#endif

  ::free(p);
}
