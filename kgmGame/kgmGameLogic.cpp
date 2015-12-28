#include "kgmGameLogic.h"
#include "kgmIGame.h"
#include "../kgmBase/kgmLog.h"

kgmGameLogic::kgmGameLogic()
{
}

kgmGameLogic::~kgmGameLogic()
{
  clear();
}

void kgmGameLogic::clear()
{
  for(kgmList<kgm_ptr<kgmUnit> >::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    (*i)->remove();
//    (*i)->release();
  }

  m_objects.clear();
}

bool kgmGameLogic::add(kgm_ptr<kgmUnit> u)
{
  if(u)
  {
    m_objects.push_back(u);

    u->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgm_ptr<kgmActor> a)
{
  if(a)
  {
    m_objects.push_back(kgm_ptr_cast<kgmUnit, kgmActor>(a));

    a->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgm_ptr<kgmEffect> e)
{
  if(e)
  {
    m_objects.push_back(kgm_ptr_cast<kgmUnit, kgmEffect>(e));

    e->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgm_ptr<kgmSensor> sn)
{
  if(sn)
  {
    m_objects.push_back(kgm_ptr_cast<kgmUnit, kgmSensor>(sn));

    sn->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgm_ptr<kgmTrigger> tr)
{
  if(tr)
  {
    m_objects.push_back(kgm_ptr_cast<kgmUnit, kgmTrigger>(tr));
//    tr->increment();

    tr->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::remove(kgm_ptr<kgmUnit> o)
{
  if(!o)
    return false;

  for(int i = m_objects.length(); i > 0; i--)
  {
    if(o == m_objects[i - 1])
    {
      m_objects.erase(i - 1);

      if(o == m_gameplayer)
        m_gameplayer.reset();

      return true;
    }
  }

  return false;
}

bool kgmGameLogic::isValid(kgmUnit *go)
{
  for(kgmList< kgm_ptr<kgmUnit> >::iterator i = m_objects.begin(); i != m_objects.end(); i.next())
  {
    if(go == (*i))
      return true;
  }

  return false;
}

void kgmGameLogic::build()
{
  for(kgmList< kgm_ptr<kgmUnit> >::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmUnit* go = (*i);
    go->init();
  }
}

void kgmGameLogic::update(u32 milliseconds)
{
#ifdef EDITOR
  u32 state = kgmIGame::getGame()->gState();

  if(kgmIGame::getGame()->gState() != kgmIGame::State_Play &&
        kgmIGame::getGame()->gState() != kgmIGame::State_Edit)
    return;
#else
  if(kgmIGame::getGame()->gState() != kgmIGame::State_Play)
    return;
#endif

  kgmList< kgm_ptr<kgmUnit> >::iterator i = m_objects.begin();

  gcount = 0;

  while(i != m_objects.end())
  {
    kgmUnit* go = (*i);

    if(go->removed())
    {
      if(m_gameplayer == go)
        m_gameplayer.reset();

      i = m_objects.erase(i);
    }
    else if(go->valid())
    {
      go->update(milliseconds);
      ++i;
    }
  }

  static int tm = kgmTime::getTicks();

  if(kgmTime::getTicks() - tm > 100)
  {
    tm = kgmTime::getTicks();
  }
}

void kgmGameLogic::input(int btn, int state)
{
  if(m_gameplayer)
    m_gameplayer->input(btn, state);
}

void kgmGameLogic::collide(kgmUnit* src, kgmUnit* dst)
{

}

kgmUnit* kgmGameLogic::getObjectById(kgmString& id)
{
  for(kgmList< kgm_ptr<kgmUnit> >::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmUnit* go = (*i);

    if(go->removed() || !go->valid())
    {
      continue;
    }
    else if(id == go->getId())
    {
      return go;
    }
  }

  return null;
}

u32 kgmGameLogic::getObjects(kgmList<kgmUnit*>& objs)
{
  u32 count = 0;

  for(kgmList< kgm_ptr<kgmUnit> >::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    objs.add((*i));
    count++;
  }

  return count;
}

u32 kgmGameLogic::getObjectsByType(kgmRuntime& t, kgmList<kgmUnit*>& objs)
{
  u32 count = 0;

  for(kgmList< kgm_ptr<kgmUnit> >::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmUnit* go = (*i);

    if(!go->removed() && go->valid() && (*i)->isType(t))
    {
      objs.add((*i));
      count++;
    }
  }

  return count;
}

u32 kgmGameLogic::getObjectsByClass(kgmRuntime& t, kgmList<kgmUnit*>& objs)
{
  u32 count = 0;

  for(kgmList< kgm_ptr<kgmUnit> >::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmUnit* go = (*i);

    if(!go->removed() && go->valid() && (*i)->isClass(t))
    {
      objs.add((*i));
      count++;
    }
  }

  return count;
}
