#include "kgmResource.h"
#include "kgmIResources.h"

KGMOBJECT_IMPLEMENT(kgmResource, kgmObject);

kgmResource::kgmResource(){
 m_type = 0;
 m_uid = 0;
}

kgmResource::~kgmResource(){
  if(m_resources)
    m_resources->remove(this);
}
