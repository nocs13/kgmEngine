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
  //for(std::vector<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    (*i)->release();
  }

  m_objects.clear();

  for(kgmList<kgmGameObject*>::iterator i = m_trush.begin(); i != m_trush.end(); ++i)
  {
    (*i)->release();
  }

  m_trush.clear();

  m_gameplayer = null;
}

bool kgmGameLogic::add(kgmActor *a)
{

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
  if(o && o->isType(kgmActor::Class) && ((kgmActor*)o)->m_gameplayer)
  {
    m_gameplayer = (kgmActor*)o;
    m_objects.push_back(o);
    o->increment();
  }
  else if(o)
  {
    m_objects.push_back(o);
    o->increment();

    return true;
  }

  return false;
}

/*bool kgmGameLogic::isvalid(kgmGameObject *go)
{
//  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  for(std::vector<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    if(go == (*i))
      return true;
  }

  return false;
}*/

void kgmGameLogic::prepare()
{
  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  //for(std::vector<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
  {
    kgmGameObject* go = (*i);
    go->init();
  }

  //if(m_gameplayer)
  //  m_gameplayer->init();
}

void kgmGameLogic::update(u32 milliseconds)
{
  if(kgmIGame::getGame()->gState() != kgmIGame::State_Play)
    return;

  if(!m_gameplayer || !kgmObject::isValid(m_gameplayer))
  {
    kgmIGame::getGame()->gCommand("gameover_fail");

    return;
  }
  //else if(m_gameplayer)
  //{
  //  m_gameplayer->update(milliseconds);
  //}

  //std::vector<kgmGameObject*>::iterator i = m_objects.begin();
  kgmList<kgmGameObject*>::iterator i = m_objects.begin();

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

    if(go->removed())
    {
      if(m_gameplayer == go)
        m_gameplayer = null;

      kgmString s = go->runtime().nClass;
      m_trush.add(go);
      i = m_objects.erase(i);
    }
    else if(go->valid())
    {
      kgmString s = go->runtime().nClass;
      go->update(milliseconds);
      ++i;
    }
  }

  static int tm = kgmTime::getTicks();

  if(kgmTime::getTicks() - tm > 1000)
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
  //for(std::vector<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
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
  //for(std::vector<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
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
  //for(std::vector<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
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
  for(int i = m_trush.size(); i > 0; i--)
  {
      m_trush[i - 1]->release();
      m_trush.erase(i - 1);

      //return;
  }

  m_trush.clear();
}
