#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kgmLog.h"

void* kgm_alloc(size_t size)
{
  //void* p = malloc(size);
  void* p = calloc(size, 1);

#ifdef DEBUG
  printf("Alloc memory: %p, size: %ld.\n", p, size);
#endif

  return p;
}

void kgm_free(void* p)
{
#ifdef DEBUG
  printf("Free memory: %p.\n", p);
#endif

  free(p);
}
