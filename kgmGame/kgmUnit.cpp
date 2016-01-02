#include "kgmUnit.h"
#include "kgmIGame.h"

KGMOBJECT_IMPLEMENT(kgmUnit, kgmObject);

kgmTab<kgmString, kgmUnit*(*)(kgmIGame*)> kgmUnit::g_typ_objects;

#ifdef EDITOR
  kgmList<kgmString> kgmUnit::g_list_sensors;
  kgmList<kgmString> kgmUnit::g_list_effects;
  kgmList<kgmString> kgmUnit::g_list_actors;
  kgmList<kgmString> kgmUnit::g_list_units;
#endif

kgmUnit::kgmUnit(kgmIGame* g)
  :kgmObject(),
  m_visual(null),
  m_body(null)
{
  m_game    = g;
  m_valid   = true;
  m_remove  = false;
  m_culled  = false;
  m_visible = true;

  m_birth   = kgmTime::getTicks();
  m_timeout = -1;
}

kgmUnit::~kgmUnit()
{
  if(m_remove)
    remove();

  if(m_body)
    delete m_body;

  if(m_visual)
    delete m_visual;
}

void kgmUnit::remove()
{
  clear();

  if(getBody())
    getBody()->remove();

  if(getVisual())
    getVisual()->remove();

  m_remove  = true;
  m_valid   = false;
  m_visible = false;
}

void kgmUnit::update(u32 mls)
{
  u32 ct = kgmTime::getTicks();

  if(removed())
    return;

  if((m_timeout != -1) && ((ct - m_birth) > m_timeout))
  {
    remove();

    return;
  }

  if(getBody())
  {
    mtx4 tm;

    if(getVisual())
    {
      m_body->transform(tm);
      m_visual->getTransform() = tm;
      m_visual->update();
    }
  }
}
