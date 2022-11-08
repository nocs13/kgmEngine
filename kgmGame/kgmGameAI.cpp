#include "kgmGameAI.h"
#include "kgmUnit.h"

kgmGameAI::kgmGameAI(kgmIGame* g)
{
  m_mutex = kgmThread::mutex_create();
  m_active = false;
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

  m_active = true;
  m_thread = kgmThread::thread_create((kgmThread::Thread_Function) fn_thread, this);
  return true;
}

bool kgmGameAI::finish()
{
  m_active = false;

  kgmThread::thread_join(m_thread);

  m_thread = NULL;

  kgmThread::mutex_unlock(m_mutex);

  return true;
}

void kgmGameAI::update()
{

}

bool kgmGameAI::addType(kgmString type)
{
  auto i = m_types.begin();

  while(!i.end()) {
    if ((*i).type == type)
      return false;
    i.next();
  }

  kgmIAI::UnitType ut;

  ut.type = type;

  m_types.add(ut);

  return true;
}

bool kgmGameAI::addState(kgmString type, kgmIAI::State state)
{
  return true;
}

bool kgmGameAI::addInput(kgmString type, kgmIAI::Input input)
{
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

int kgmGameAI::fn_thread(void* m)
{
  while(((kgmGameAI*)m)->m_active)
  {
    kgmThread::mutex_lock(((kgmGameAI*)m)->m_mutex);
    ((kgmGameAI*)m)->update();
    kgmThread::mutex_unlock(((kgmGameAI*)m)->m_mutex);
    kgmThread::sleep(0);
  }

  return 0;
}