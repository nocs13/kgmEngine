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
  for(int i = 0; i < m_childs.length(); i++)
    m_childs[i]->release();
  m_childs.clear();

  if(m_body)
    m_body->release();

  if(m_visible)
    m_visual->release();
}
