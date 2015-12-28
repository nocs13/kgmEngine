#include "kgmResource.h"
#include "kgmIResources.h"

#ifdef DEBUG
#include "kgmLog.h"
#endif

KGMOBJECT_IMPLEMENT(kgmResource, kgmObject);

kgmResource::kgmResource()
{
  m_type = 0;
  m_uid = 0;

  m_lock = false;

  m_references = 1;
}

kgmResource::~kgmResource()
{
}
