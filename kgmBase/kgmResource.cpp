#include "kgmResource.h"
#include "kgmIResources.h"

#ifdef DEBUG
#include "kgmLog.h"
#endif

kgmResource::kgmResource()
{
  m_uid = 0;
  m_type = 0;
  m_group = 0;
}

kgmResource::~kgmResource()
{
}
