#include "kgmGameLogic.h"
#include "kgmIGame.h"
#include "../kgmBase/kgmLog.h"

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
    (*i)->remove();
    (*i)->release();
  }

  m_objects.clear();

  for(kgmList<kgmGameObject*>::iterator i = m_inputs.begin(); i != m_inputs.end(); ++i)
  {
    (*i)->release();
  }

  m_inputs.clear();

  for(kgmList<kgmGameObject*>::iterator i = m_trush.begin(); i != m_trush.end(); ++i)
  {
    (*i)->release();
  }

  m_trush.clear();

  m_gameplayer = null;
}

void kgmGameLogic::build()
{

}

bool kgmGameLogic::add(kgmSensor *a)
{
  return false;
}

bool kgmGameLogic::add(kgmTrigger *a)
{
  return false;
}

bool kgmGameLogic::add(kgmGameObject *o, bool input)
{
  if(o && o->isType(kgmActor::Class) && ((kgmActor*)o)->m_gameplayer)
  {
    m_gameplayer = (kgmActor*)o;
    m_objects.push_back(o);
    o->increment();

    if(kgmObject::isValid(o))
    {
      int k = 0;
    }

    return true;
  }
  else if(o)
  {
    m_objects.push_back(o);
    o->increment();

    if(input)
    {
      m_inputs.add(o);
      o->increment();
    }

    return true;
  }

  return false;
}

bool kgmGameLogic::isValid(kgmGameObject *go)
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); i.next())
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

  kgmList<kgmGameObject*>::iterator i = m_objects.begin();

  gcount = 0;
  while(i != m_objects.end())
  {
    kgmGameObject* go = (*i);

    if(!kgmObject::isValid(go))
    {
      i = m_objects.erase(i);

      if(m_gameplayer == go)
        m_gameplayer = null;

      continue;
    }
    else if(go->removed())
    {
      if(m_gameplayer == go)
        m_gameplayer = null;

      m_trush.add(go);
      i = m_objects.erase(i);
    }
    else if(go->valid())
    {
      go->update(milliseconds);
      ++i;
    }

    if(go->isType(kgmActor::Class))
    {
      kgmActor* a = (kgmActor*)go;
      if(a->m_gameplayer)
        gcount++;
    }
  }

  static int tm = kgmTime::getTicks();

  if(kgmTime::getTicks() - tm > 100)
  {
    trush();
    tm = kgmTime::getTicks();
  }
}

void kgmGameLogic::input(int btn, int state)
{
  if(m_gameplayer && kgmObject::isValid(m_gameplayer))
  {
    m_gameplayer->input(btn, state);
  }
//  else
  {
    for(int i = 0; i < m_inputs.length(); i++)
    {
      ((kgmActor*)m_inputs[i])->input(btn, state);
    }
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

u32 kgmGameLogic::getObjects(kgmList<kgmGameObject*>& objs)
{
  u32 count = 0;

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    objs.add((*i));
    count++;
  }

  return count;
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

void kgmGameLogic::trush()
{
  for(kgmList<kgmGameObject*>::iterator i = m_trush.begin(); i != m_trush.end(); ++i)
  {
    (*i)->release();
  }

  m_trush.clear();
}
