#include "kgmIResources.h"
#include "kgmResource.h"

void kgmIResources::lock(kgmResource *r)
{
  r->lock();
}

void kgmIResources::unlock(kgmResource *r)
{
  r->unlock();
}
