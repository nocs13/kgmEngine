#include "kgmResource.h"
#include "kgmIResources.h"

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
    g_resources->remove(this);

  kgmObject::release();
}
