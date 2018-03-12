#include "kgmGameThreader.h"

kgmGameThreader::kgmGameThreader()
{
  m_locked = false;
  m_active = true;

  memset(m_threaders, 0, sizeof(m_threaders));

  m_thread = kgmThread::thread_create(kgmGameThreader::threader, this);
}

kgmGameThreader::~kgmGameThreader()
{
  finish();
}

bool kgmGameThreader::add(THREADER_FUNCTION fn, void *obj)
{
  if(!fn || locked())
    return false;

  for (s32 i = 0; i < MAX_THREADERS; i++)
  {
    if (m_threaders[i].funtion == null)
    {
      m_threaders[i] = {fn, obj};

      return true;
    }
  }

  return false;
}

bool kgmGameThreader::remove(THREADER_FUNCTION fn)
{
  if(!fn || locked())
    return false;

  for (s32 i = 0; i < MAX_THREADERS; i++)
  {
    if (m_threaders[i].funtion == fn)
    {
      m_threaders[i] = {null, null};

      return true;
    }
  }

  return false;
}

bool kgmGameThreader::finish()
{
  m_active = false;
  kgmThread::thread_join(m_thread);

  return true;
}

void kgmGameThreader::ready()
{
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

    gt->m_locked = true;

    for(u32 i = 0; i < MAX_THREADERS; i++)
    {
      if(gt->m_threaders[i].funtion)
        gt->m_threaders[i].funtion(gt->m_threaders[i].object);
    }

    gt->m_locked = false;
  }

  return 0;
}
