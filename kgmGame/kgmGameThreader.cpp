#include "kgmGameThreader.h"

kgmGameThreader::kgmGameThreader()
{
  m_active = true;

  for (u32 i = 0; i < MAX_THREADS; i++)
  {
    memset(m_threads[i].workers, 0, sizeof(Worker) * MAX_WORKERS);
    m_threads[i].count  = 0;
    m_threads[i].gt     = this;
    m_threads[i].mutex  = kgmThread::mutex_create();
    m_threads[i].thread = kgmThread::thread_create((kgmThread::Thread_Function) kgmGameThreader::threader, &m_threads[i]);
  }
}

kgmGameThreader::~kgmGameThreader()
{
  finish();
}

bool kgmGameThreader::add(THREADER_FUNCTION fn, void *ob)
{
  if(!fn)
    return false;

  Thread *thread = &m_threads[0];

  for(u32 i = 1; i < MAX_THREADS; i++)
    if(m_threads[i].count < thread->count)
      thread = &m_threads[i];

  if(thread->count == MAX_WORKERS)
    return false;

  kgmThread::mutex_lock(thread->mutex);

  for (s32 i = 0; i < MAX_WORKERS; i++)
  {
    if(thread->workers[i].funtion == null)
    {
      thread->workers[i].funtion = fn;
      thread->workers[i].object  = ob;
      thread->count++;
      break;
    }
  }

  kgmThread::mutex_unlock(thread->mutex);

  return true;
}

bool kgmGameThreader::remove(THREADER_FUNCTION fn)
{
  if(!fn)
    return false;

  Thread *thread = null;

  u32 wi = 0;

  for(u32 i = 0; i < MAX_THREADS; i++)
  {
    for(u32 j = 1; j < MAX_WORKERS; j++)
    {
      if(m_threads[i].workers[j].funtion == fn)
      {
        thread = &m_threads[i];
        wi = j;

        break;
      }
    }
  }

  if(!thread)
    return false;

  kgmThread::mutex_lock(thread->mutex);

  thread->workers[wi].funtion = null;
  thread->workers[wi].object  = null;
  thread->count--;

  kgmThread::mutex_unlock(thread->mutex);

  return true;
}

bool kgmGameThreader::finish()
{
  if(!m_active)
    return false;

  m_active = false;

  for (u32 i = 0; i < MAX_THREADS; i++)
  {
    kgmThread::thread_join(m_threads[i].thread);
    kgmThread::mutex_free(m_threads[i].mutex);
  }

  return true;
}

int kgmGameThreader::threader(kgmGameThreader::Thread* t)
{
  if(!t)
    return -1;

  kgmGameThreader* gt = (kgmGameThreader*)t->gt;

  while(gt->m_active)
  {
    kgmThread::mutex_lock(t->mutex);

    for(u32 i = 0; i < MAX_WORKERS; i++)
    {
      if(t->workers[i].funtion)
        t->workers[i].funtion(t->workers[i].object);
    }

    kgmThread::mutex_unlock(t->mutex);

    kgmThread::sleep(0);
  }

  return 0;
}
