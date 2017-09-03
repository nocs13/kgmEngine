#include "kgmGameThreader.h"

kgmGameThreader::kgmGameThreader()
{
  m_locked = false;
  m_active = true;

  m_count = 0;

  m_thread = kgmThread::thread_create(kgmGameThreader::threader, this);
}

kgmGameThreader::~kgmGameThreader()
{
  finish();
}

bool kgmGameThreader::add(THREADER_FUNCTION fn, void *obj)
{
  if(!fn || locked() || m_count >= MAX_THREADERS)
    return false;

  m_threaders[m_count++] = {fn, obj};

  return true;
}

bool kgmGameThreader::finish()
{
  m_active = false;
  kgmThread::thread_join(m_thread);

  return true;
}

void kgmGameThreader::ready()
{
  m_ready = true;
  m_locked = true;
}

int kgmGameThreader::threader(void *v)
{
  if(!v)
    return -1;

  kgmGameThreader* gt = (kgmGameThreader*)v;

  while(gt->m_active)
  {
    kgmThread::sleep(0);

    if(!gt->m_ready)
      continue;

    gt->m_locked = true;

    for(u32 i = 0; i < gt->m_count; i++)
    {
      //gt->m_threaders[i].funtion(gt->m_threaders[i].object);
    }

    gt->m_count = 0;
    gt->m_ready = false;
    gt->m_locked = false;
  }

  return 0;
}
