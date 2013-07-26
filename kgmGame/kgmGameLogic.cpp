#include "kgmGameLogic.h"
#include "kgmIGame.h"

kgmGameLogic::kgmGameLogic()
{
  m_gameplayer = null;
}

kgmGameLogic::~kgmGameLogic()
{
  clear();
}

void kgmGameLogic::clear()
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {

    (*i)->exit();
    (*i)->release();
  }

  m_objects.clear();

  m_gameplayer = null;
}

bool kgmGameLogic::add(kgmActor *a)
{
  if(a)
  {
    if(a->m_gameplayer)
      m_gameplayer = a;

    m_objects.add(a);
    a->increment();

    return true;
  }

  return false;
}

bool kgmGameLogic::add(kgmSensor *a)
{
  return false;
}

bool kgmGameLogic::add(kgmTrigger *a)
{
  return false;
}

bool kgmGameLogic::add(kgmGameObject *o)
{
  if(o)
  {
    m_objects.add(o);
    o->increment();

    return true;
  }

  return false;
}

bool kgmGameLogic::isvalid(kgmGameObject *go)
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    if(go == (*i))
      return true;
  }

  return false;
}

void kgmGameLogic::prepare()
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);
    go->init();
  }
}

void kgmGameLogic::update(u32 milliseconds)
{
  if(kgmIGame::getGame()->gState() != kgmIGame::State_Play)
    return;

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  //for(int i = m_objects.size(); i > 0; --i)
  {
    kgmGameObject* go = (*i);
    //kgmGameObject* go = m_objects[i - 1];

    if(go->removed())
    {
      if(m_gameplayer == go)
        m_gameplayer = null;

      go->release();

      i = m_objects.erase(i);
      //m_objects.erase(i - 1);

      break;
    }
    else if(isvalid(go) && go->valid())
    {
      go->update(milliseconds);
    }
  }
}

void kgmGameLogic::input(int btn, int state)
{
  if(m_gameplayer && isvalid(m_gameplayer))
  {
    m_gameplayer->input(btn, state);
  }
  else
  {
  }
}

void kgmGameLogic::action(ACTION, kgmObject*, kgmString)
{

}

void kgmGameLogic::collide(kgmGameObject* src, kgmGameObject* dst)
{

}

kgmGameObject* kgmGameLogic::getObjectById(kgmString id)
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

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

u32 kgmGameLogic::getObjectsByType(kgmRuntime& t, kgmList<kgmGameObject*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    if(!go->removed() && go->valid() && (*i)->isType(t))
    {
      objs.add((*i));
      count++;
    }
  }

  return count;
}

u32 kgmGameLogic::getObjectsByClass(kgmRuntime& t, kgmList<kgmGameObject*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    if(!go->removed() && go->valid() && (*i)->isClass(t))
    {
      objs.add((*i));
      count++;
    }
  }

  return count;
}
