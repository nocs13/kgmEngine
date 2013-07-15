#include "kgmGameObject.h"
KGMOBJECT_IMPLEMENT(kgmGameObject, kgmObject);

kgmGameObject::kgmGameObject()
  :kgmObject(),
  m_visual(null),
  m_body(null),
  m_parent(null)
{
  m_valid  = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_birth   = kgmTime::getTicks();
  m_timeout = -1;
}

kgmGameObject::~kgmGameObject()
{
  if(m_body)
    m_body->release();

  if(m_visible)
    m_visual->release();
}

void kgmGameObject::update(u32 mls)
{
  u32 ct = kgmTime::getTicks();

  if((m_timeout != -1) && ((ct - m_birth) > m_timeout))
  {
    setParent(null);

    if(getBody())
      getBody()->remove();

    if(getVisual())
      getVisual()->remove();

    disable();
    remove();

    return;
  }

  if(m_parent && m_parent->getBody())
  {
    if(getBody())
    {
      vec3 v = m_parent->getBody()->m_position + m_position;
      vec3 r = m_parent->getBody()->m_rotation + m_rotation;

      //getBody()->translate(v);
      //getBody()->rotate(r);
    }
  }

  if(getBody())
  {
    mtx4 tm;

    if(getVisual())
    {
      m_body->transform(tm);
      m_visual->m_transform = tm;
    }
  }
}
