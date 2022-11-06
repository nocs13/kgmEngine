#include "kgmGameAI.h"

kgmGameAI::kgmGameAI(kgmIGame* g)
{
  m_mutex = kgmThread::mutex_create();
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

  return true;
}

bool kgmGameAI::addState(kgmString type, kgmIAI::State state)
{

  return true;
}

bool kgmGameAI::addInput(kgmString type, kgmIAI::Input inpout)
{

  return true;
}

bool kgmGameAI::addUnit(kgmUnit* unit)
{

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