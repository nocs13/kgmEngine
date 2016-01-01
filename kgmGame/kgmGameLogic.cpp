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
  m_objects.clear();
}

bool kgmGameLogic::add(kgmUnit* u)
{
  if(!u)
    return false;

  m_objects.push_back(u);

  u->init();

  return true;
}

bool kgmGameLogic::add(kgmActor* a)
{
  if(!a)
    return false;

  m_objects.push_back(a);

  a->init();

  return true;

}

bool kgmGameLogic::add(kgmEffect* e)
{
  if(!e)
    return false;

  m_objects.push_back(e);

  e->init();

  return true;
}

bool kgmGameLogic::add(kgmSensor* s)
{
  if(!s)
    return false;

  m_objects.push_back(s);

  s->init();

  return true;
}

bool kgmGameLogic::add(kgmTrigger* t)
{
  if(!t)
    return false;

  m_objects.push_back(t);

  t->init();

  return true;
}

bool kgmGameLogic::remove(kgmUnit* o)
{
  if(!o)
    return false;

  for(int i = m_objects.length(); i > 0; i--)
  {
    if(o == m_objects[i - 1])
    {
      m_objects.erase(i - 1);

      if(o == m_gameplayer)
        m_gameplayer = null;

      return true;
    }
  }

  return false;
}

bool kgmGameLogic::isValid(kgmUnit *go)
{
  for(kgmList<kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); i.next())
  {
    if(go == (*i))
      return true;
  }

  return false;
}

void kgmGameLogic::build()
{
  for(kgmList<kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
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

  kgmList<kgmUnit*>::iterator i = m_objects.begin();

  gcount = 0;

  while(i != m_objects.end())
  {
    kgmUnit* go = (*i);

    if(go->removed())
    {
      if(m_gameplayer == go)
        m_gameplayer = null;

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
  for(kgmList<kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
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

  for(kgmList<kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    objs.add((*i));
    count++;
  }

  return count;
}

u32 kgmGameLogic::getObjectsByType(kgmRuntime& t, kgmList<kgmUnit*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
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

  for(kgmList<kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
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
