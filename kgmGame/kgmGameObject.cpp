#include "kgmGameObject.h"
KGMOBJECT_IMPLEMENT(kgmGameObject, kgmObject);

kgmGameObject::kgmGameObject()
 :m_visual(null),
  m_body(null),
  m_parent(null)
{
  m_remove  = false;
  m_culled  = false;
  m_enable  = true;
  m_active  = true;
  m_visible = true;

}

kgmGameObject::~kgmGameObject()
{

}
