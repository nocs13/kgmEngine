#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kgmLog.h"

void* kgm_alloc(size_t size)
{
  void* p = malloc(size);

#ifdef DEBUG
  kgm_log() << "Alloc memory: " << p << " size: " << (s32)size << "\n";
#endif

  return p;
}

void kgm_free(void* p)
{
#ifdef DEBUG
  kgm_log() << "Free memory: " << p << "\n";
#endif

  free(p);
}
