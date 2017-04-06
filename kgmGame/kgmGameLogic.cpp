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
}

bool kgmGameLogic::add(kgmUnit* u)
{
  if(!u)
    return false;

  m_objects.set(u->getName(), u);

  u->init();

  return true;
}

bool kgmGameLogic::remove(kgmUnit* o)
{
  if(!o)
    return false;

  for(kgmHArray<kgmString, kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmUnit* u = (*i);

    if(o == u)
    {
      i = m_objects.erase(i);

      if(o == m_gameplayer)
        m_gameplayer = null;

      u->exit();

      return true;
    }
  }

  return false;
}

bool kgmGameLogic::isValid(kgmUnit *go)
{
  for(kgmHArray<kgmString, kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    if(go == (*i))
      return true;
  }

  return false;
}

void kgmGameLogic::build()
{
  for(kgmHArray<kgmString, kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmUnit* go = (*i);
    go->init();
  }
}

void kgmGameLogic::update(u32 milliseconds)
{
  kgmHArray<kgmString, kgmUnit*>::iterator i = m_objects.begin();

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
  kgmUnit* go = null;

  if(m_objects.exist(id))
    go = m_objects.get(id);

  if(go->removed() || !go->valid())
  {
    go = null;
  }

  return go;
}

u32 kgmGameLogic::getObjects(kgmList<kgmUnit*>& objs)
{
  u32 count = 0;

  for(kgmHArray<kgmString, kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    objs.add((*i));

    count++;
  }

  return count;
}

u32 kgmGameLogic::getObjectsByClass(kgmString& t, kgmList<kgmUnit*>& objs)
{
  u32 count = 0;

  for(kgmHArray<kgmString, kgmUnit*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
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
