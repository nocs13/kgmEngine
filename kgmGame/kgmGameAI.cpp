#include "kgmGameAI.h"
#include "kgmUnit.h"
#include "kgmGameBase.h"
#include "kgmGameScript.h"

#include "../kgmSystem/kgmTime.h"

kgmGameAI::kgmGameAI(kgmIGame* g)
{
  m_game = g;
  m_mutex = kgmThread::mutex_create();
  m_active = false;
  m_thread = NULL;
}

kgmGameAI::~kgmGameAI()
{
  m_active = false;
  kgmThread::thread_join(m_thread);
  kgmThread::mutex_free(m_mutex);

  m_units.clear();
  m_types.clear();
}

bool kgmGameAI::start()
{
  if (m_active)
    return false;

  m_thread = kgmThread::thread_create((kgmThread::Thread_Function) fn_thread, this);

  return true;
}

bool kgmGameAI::finish()
{
  m_active = false;

  if ((m_thread != NULL)  && kgmThread::thread_join(m_thread))
    kgmThread::thread_join(m_thread);

  m_thread = NULL;

  kgmThread::mutex_unlock(m_mutex);

  return true;
}

void kgmGameAI::clean()
{
  finish();

  m_units.clear();
}

void kgmGameAI::update()
{
  if (m_game) {
    kgmIScript* s = m_game->getScript();

    auto st = m_game->gState();

    if (s && (st == kgmIGame::State_Play))
    {
      s->call("main_onplay", "");
    }
  }
}

bool kgmGameAI::addType(kgmString type)
{
  if (getType(type))
    return false;

  kgmIAI::UnitType ut;

  ut.type = type;

  m_types.add(ut);

  return true;
}

bool kgmGameAI::addState(kgmString type, kgmIAI::State state)
{
  UnitType* ut = getType(type);

  if (!ut)
    return false;

  State* st = getState(ut, state.id);

  if (st)
    return false;

  ut->states.add(state);

  return true;
}

bool kgmGameAI::addInput(kgmString type, kgmIAI::Input input)
{
  UnitType* ut = getType(type);

  if (!ut)
    return false;

  Input* i = getInput(ut, input.input);

  if (i)
    return false;

  ut->inputs.add(input);

  return true;
}

bool kgmGameAI::addUnit(kgmUnit* unit)
{
  if (!unit)
    return false;

  auto i = m_units.begin();

  while(!i.end()) {
    if ((*i).unit == unit)
      return false;
    i.next();
  }

  kgmString st = unit->getClass();
  UnitType* ut = null;

  auto j = m_types.begin();

  while(!j.end()) {
    if ((*j).type == st) {
      ut = &(*j);
      break;
    }

    j.next();
  }

  if (!ut)
    return false;

  kgmIAI::Unit u;

  u.unit = unit;
  u.type = ut;

  m_units.add(u);

  return true;
}

kgmGameAI::UnitType* kgmGameAI::getType(kgmString type)
{
  auto i = m_types.begin();

  while(!i.end()) {
    if ((*i).type == type) {
      return &(*i);
    }

    i.next();
  }

  return null;
}

kgmGameAI::State* kgmGameAI::getState(UnitType* ut, kgmString s)
{
  if (!ut)
    return null;

  auto i = ut->states.begin();

  while(!i.end()) {
    if ((*i).id == s)
      return &(*i);
    i.next();
  }

  return null;
}

kgmGameAI::Input*  kgmGameAI::getInput(UnitType* ut, u32 in)
{
  if (!ut)
    return null;

  auto i = ut->inputs.begin();

  while(!i.end()) {
    if ((*i).input1 == in)
      return &(*i);
    i.next();
  }

  return null;
}

int kgmGameAI::fn_thread(void* m)
{
  u32 ms = 50;

  ((kgmGameAI*)m)->m_active = true;

  while(((kgmGameAI*)m)->m_active)
  {
    u32 t = kgmTime::getTicks();

    kgmThread::mutex_lock(((kgmGameAI*)m)->m_mutex);

    ((kgmGameAI*)m)->update();

    kgmThread::mutex_unlock(((kgmGameAI*)m)->m_mutex);

    u32 d = kgmTime::getTicks() - t;

    if (d < ms)
      kgmThread::sleep(ms - d);
    else
      kgmThread::sleep(0);
  }

  return 0;
}
