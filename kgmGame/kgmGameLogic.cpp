#include "kgmGameLogic.h"
#include "kgmIGame.h"

kgmGameLogic::kgmGameLogic()
{
  m_levlogic   = null;
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

  if(m_levlogic)
    delete m_levlogic;

  m_objects.clear();

  m_gameplayer = null;
  m_levlogic   = null;
}

bool kgmGameLogic::add(kgmActor *a)
{
  if(a)
  {
    if(a->m_gameplayer)
      m_gameplayer = a;

    m_objects.add(a);
    a->increment();
    //a->init();

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
    //o->init();

    return true;
  }

  return false;
}

bool kgmGameLogic::chooseLogic(kgmString s)
{
  LogicOfLevel* ll = getLogic(s);

  if(!ll)
    return false;

  if(m_levlogic)
    delete m_levlogic;

  m_levlogic = ll;

  return true;
}

bool kgmGameLogic::validity(kgmGameObject *go)
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

  if(m_levlogic)
    m_levlogic->init();
}

void kgmGameLogic::update(u32 milliseconds)
{
  if(kgmIGame::getGame()->gState() != kgmIGame::State_Play)
    return;

  if(m_levlogic)
    m_levlogic->update();

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    if(go->valid())
    {
      go->update(milliseconds);
    }
  }

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);

    if(go->removed())
    {
      if(m_gameplayer == go)
        m_gameplayer = null;

      go->release();
      m_objects.erase(i);
    }
  }
}

void kgmGameLogic::input(int btn, int state)
{
  if(m_gameplayer && validity(m_gameplayer))
  {
    m_gameplayer->input(btn, state);
  }
  else
  {
  }
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

kgmGameLogic::LogicOfLevel* kgmGameLogic::getLogic(kgmString)
{

}
