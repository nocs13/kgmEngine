#include "kgmGameThreader.h"

kgmGameThreader::kgmGameThreader()
{
  m_active = true;

  memset(m_threaders, 0, sizeof(m_threaders));

  m_mutex  = kgmThread::mutex_create();

  kgmThread::sleep(1);

  m_thread = kgmThread::thread_create(kgmGameThreader::threader, this);
}

kgmGameThreader::~kgmGameThreader()
{
  finish();

  kgmThread::mutex_free(m_mutex);
}

bool kgmGameThreader::add(THREADER_FUNCTION fn, void *obj)
{
  if(!fn)
    return false;

  kgmThread::mutex_lock(m_mutex);

  bool result = false;

  for (s32 i = 0; i < MAX_THREADERS; i++)
  {
    if (m_threaders[i].funtion == null)
    {
      m_threaders[i] = {fn, obj};

      result = true;
    }
  }

  kgmThread::mutex_unlock(m_mutex);

  return result;
}

bool kgmGameThreader::remove(THREADER_FUNCTION fn)
{
  if(!fn)
    return false;

  kgmThread::mutex_lock(m_mutex);

  bool result = false;

  for (s32 i = 0; i < MAX_THREADERS; i++)
  {
    if (m_threaders[i].funtion == fn)
    {
      m_threaders[i] = {null, null};

      result = true;
    }
  }

  kgmThread::mutex_unlock(m_mutex);

  return result;
}

bool kgmGameThreader::finish()
{
  m_active = false;
  kgmThread::thread_join(m_thread);

  return true;
}

int kgmGameThreader::threader(void *v)
{
  if(!v)
    return -1;

  kgmGameThreader* gt = (kgmGameThreader*)v;

  while(gt->m_active)
  {
    kgmThread::sleep(0);

    kgmThread::mutex_lock(gt->m_mutex);

    for(u32 i = 0; i < MAX_THREADERS; i++)
    {
      if(gt->m_threaders[i].funtion)
        gt->m_threaders[i].funtion(gt->m_threaders[i].object);
    }

    kgmThread::mutex_unlock(gt->m_mutex);
  }

  return 0;
}
