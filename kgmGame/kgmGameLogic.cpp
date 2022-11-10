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

  m_objects.set(u->getName(), u);

  return true;
}

bool kgmGameLogic::remove(kgmUnit* o)
{
  if(!o)
    return false;

  for(kgmTab<kgmString, kgmUnit*>::iterator i = m_objects.begin(); !i.end(); ++i)
  {
    kgmUnit* u = (*i);

    if(o == u)
    {
      i = m_objects.erase(i);

      if(o == m_gameplayer)
        m_gameplayer = null;

      u->finish();

      return true;
    }
  }

  return false;
}

bool kgmGameLogic::isValid(kgmUnit *go)
{
  for(kgmTab<kgmString, kgmUnit*>::iterator i = m_objects.begin(); !i.end(); ++i)
  {
    if(go == (*i))
      return true;
  }

  return false;
}

void kgmGameLogic::build()
{
  /*for(kgmTab<kgmString, kgmUnit*>::iterator i = m_objects.begin(); !i.end(); ++i)
  {
    kgmUnit* go = (*i);
    go->init();
  }*/
}

void kgmGameLogic::update(u32 milliseconds)
{
  kgmTab<kgmString, kgmUnit*>::iterator i = m_objects.begin();

  gcount = 0;

  while(!i.end())
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
}

void kgmGameLogic::collide(kgmUnit* src, kgmUnit* dst)
{

}

kgmUnit* kgmGameLogic::getObject(kgmString& id)
{
  kgmUnit* go = null;

  kgmTab<kgmString, kgmUnit*>::iterator i = m_objects.get(id);

  if(!i.isEnd())
    go = (*i);

  if(go->removed() || !go->valid())
  {
    go = null;
  }

  return go;
}
