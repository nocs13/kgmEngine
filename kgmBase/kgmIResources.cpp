#include "kgmIResources.h"
#include "kgmResource.h"

kgmIResources* kgmIResources::m_manager = null;

void kgmIResources::lock(kgmResource *r)
{
  r->lock();
}

void kgmIResources::unlock(kgmResource *r)
{
  r->unlock();
}
