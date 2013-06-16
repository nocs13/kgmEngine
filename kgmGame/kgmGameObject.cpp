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
  for(int i = 0; i < m_childs.length(); i++)
    m_childs[i]->release();
  m_childs.clear();

  if(m_body)
    m_body->release();

  if(m_visible)
    m_visual->release();
}

void kgmGameObject::update(u32 mls)
{
  u32 ct = kgmTime::getTicks();

  if(m_childs.size())
  {
    for(int i = m_childs.length(); i > 0; i--)
    {
      kgmGameObject* go = m_childs[i - 1];

      if(go->removed())
      {
        //m_childs.erase(i - 1);

        go->setParent(null);
        go->release();
      }
    }
  }

  if((m_timeout != -1) && ((ct - m_birth) > m_timeout))
  {
    for(int i = 0; i < m_childs.length(); i++)
    {
      m_childs[i]->disable();
      m_childs[i]->remove();
      m_childs[i]->release();
    }

    m_childs.clear();

    if(getBody())
      getBody()->remove();

    if(getVisual())
      getVisual()->remove();

    disable();
    remove();

    return;
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
