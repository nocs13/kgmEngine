#include "kgmGameObject.h"
KGMOBJECT_IMPLEMENT(kgmGameObject, kgmObject);

kgmTab<kgmString, kgmGameObject*(*)(kgmIGame*)> kgmGameObject::g_typ_objects;

kgmGameObject::kgmGameObject(kgmIGame* g)
  :kgmObject(),
  m_visual(null),
  m_body(null),
  m_parent(null)
{
  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_birth   = kgmTime::getTicks();
  m_timeout = -1;
}

kgmGameObject::~kgmGameObject()
{
  if(m_remove)
    remove();

  if(m_body)
    m_body->release();

  if(m_visual)
    m_visual->release();
}

void kgmGameObject::remove()
{
  setParent(null);

  if(getBody())
    getBody()->remove();

  if(getVisual())
    getVisual()->remove();

  m_remove  = true;
  m_valid   = false;
  m_visible = false;
}

void kgmGameObject::update(u32 mls)
{
  u32 ct = kgmTime::getTicks();

  if(removed())
    return;

  if((m_timeout != -1) && ((ct - m_birth) > m_timeout))
  {
    remove();

    exit();

    return;
  }

  if(getParent())
  {
    if(!kgmObject::isValid(getParent()) || getParent()->removed())
    {
      remove();
    }
  }

  if(getBody())
  {
    mtx4 tm;

    if(getVisual())
    {
      m_body->transform(tm);
      m_visual->m_transform = tm;
      m_visual->update();
    }
  }
  else
  {
    mtx4 tr, rt;

    if(getVisual())
    {
      tr.translate(m_position);
      rt.rotate(m_rotation);
      m_visual->m_transform = rt * tr;
      m_visual->update();
    }
  }
}
