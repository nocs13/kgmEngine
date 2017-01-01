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

bool kgmGameLogic::add(kgmGameNode* u)
{
  if(!u)
    return false;

  m_objects.push_back(u);

  u->init();

  return true;
}

bool kgmGameLogic::remove(kgmGameNode* o)
{
  if(!o)
    return false;

  for(int i = m_objects.length(); i > 0; i--)
  {
    kgmGameNode* u = m_objects[i - 1];

    if(o == u)
    {
      m_objects.erase(i - 1);

      if(o == m_gameplayer)
        m_gameplayer = null;

      u->exit();

      return true;
    }
  }

  return false;
}

bool kgmGameLogic::isValid(kgmGameNode *go)
{
  for(kgmList<kgmGameNode*>::iterator i = m_objects.begin(); i != m_objects.end(); i.next())
  {
    if(go == (*i))
      return true;
  }

  return false;
}

void kgmGameLogic::build()
{
  for(kgmList<kgmGameNode*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameNode* go = (*i);
    go->init();
  }
}

void kgmGameLogic::update(u32 milliseconds)
{
  kgmList<kgmGameNode*>::iterator i = m_objects.begin();

  gcount = 0;

  while(i != m_objects.end())
  {
    kgmGameNode* go = (*i);

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

void kgmGameLogic::collide(kgmGameNode* src, kgmGameNode* dst)
{

}

kgmGameNode* kgmGameLogic::getObjectById(kgmString& id)
{
  for(kgmList<kgmGameNode*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameNode* go = (*i);

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

u32 kgmGameLogic::getObjects(kgmList<kgmGameNode*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmGameNode*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    objs.add((*i));
    count++;
  }

  return count;
}

u32 kgmGameLogic::getObjectsByClass(kgmString& t, kgmList<kgmGameNode*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmGameNode*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameNode* go = (*i);

    if(!go->removed() && go->valid() && (*i)->isClass(t))
    {
      objs.add((*i));
      count++;
    }
  }

  return count;
}
