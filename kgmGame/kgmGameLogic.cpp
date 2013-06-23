#include "kgmGameLogic.h"
#include "kgmIGame.h"

kgmGameLogic::kgmGameLogic()
{
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
    a->init();

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
    o->init();

    return true;
  }

  return false;
}

void kgmGameLogic::update(u32 milliseconds)
{
  if(kgmIGame::getGame()->gState() != kgmIGame::State_Play)
    return;

  for(kgmList<kgmGameObject*>::iterator i = m_objects.begin(); i != m_objects.end(); i++)
  {
    kgmGameObject* go = (*i);

    if(go->removed())
    {
      go->release();
      m_objects.erase(i);

      if(m_gameplayer == go)
        m_gameplayer = null;
    }
    else if(go->valid())
    {
      go->update(milliseconds);
    }
  }
}

void kgmGameLogic::input(int btn, int state)
{
  if(m_gameplayer)
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

/*kgmGameObject* kgmGameLogic::createGameObject(kgmString t)
{
  return null;
}*/
