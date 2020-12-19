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

  m_lock = false;

  m_references = 1;
}

kgmResource::~kgmResource()
{
}

void kgmResource::release()
{
  if (m_lock)
    return;
  
  if(m_references > 0)
    m_references--;

  if (m_references == 0)
    kgmIResources::manager()->remove(this);
}

