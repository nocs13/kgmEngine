#include "kgmResource.h"
#include "kgmIResources.h"

#ifdef DEBUG
#include "kgmLog.h"
#endif

KGMOBJECT_IMPLEMENT(kgmResource, kgmObject);

kgmIResources* kgmResource::g_resources = null;

kgmResource::kgmResource(){
 m_type = 0;
 m_uid = 0;
}

kgmResource::~kgmResource(){
}

void kgmResource::release()
{
  if(g_resources && references() == 1)
  {
#ifdef DEBUG
    kgm_log() << "\nResource release:";

    if(m_id.length() > 0)
      kgm_log() << m_id.data() << "\n";
#endif
    g_resources->remove(this);
  }

  kgmObject::release();
}
