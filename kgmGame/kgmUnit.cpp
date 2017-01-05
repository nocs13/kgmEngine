#include "kgmUnit.h"
#include "kgmIGame.h"

kgmTab<kgmString, kgmUnit*(*)(kgmIGame*)> kgmUnit::g_typ_objects;

kgmTab<kgmString, kgmUnit::ActionCallback> kgmUnit::g_list_action;

kgmUnit::kgmUnit(kgmIGame* g)
  :kgmGameNode(g)
{
  m_type = UNIT;
}

kgmUnit::~kgmUnit()
{
  if(m_body)
    delete m_body;

  if(m_visual)
    delete m_visual;
}

void kgmUnit::update(u32 mls)
{
  u32 ct = kgmTime::getTicks();

  if(removed())
    return;

  if((getLiving() != -1) && (timeout() > getLiving()))
  {
    remove();

    return;
  }

  if(m_action.id.length() > 0)
  {
    if(!m_action.callback)
      m_action.callback = getActionCallback(m_action.id);

    if(m_action.callback)
      m_action.callback(game(), this, &m_action);
  }

  if(m_visual)
  {
    mtx4 tm;

    if(m_body)
    {
      m_body->transform(tm);
    }
    else
    {
      mtx4 rt, tr;

      tr.identity();
      //tr.translate(m_position);
      rt.identity();
      //rt.rotate(m_rotation);

      tm = rt * tr;
    }

    m_visual->getTransform() = tm;
    m_visual->update();
  }
}
