#include "kgmThread.h"
#include "kgmSystem.h"

KGMOBJECT_IMPLEMENT(kgmThread, kgmObject);

void kgmThread::thread(kgmThread *p)
{
  if(!p)
    return;

#ifdef WIN32

#else
  if(p->m_canselable)
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif

  p->run();
}

kgmThread::kgmThread()
{
  m_thread = 0;
  m_canselable = false;
}

kgmThread::~kgmThread()
{
  if(m_thread)
    kill();
}

bool kgmThread::exec(bool canselable, Priority pr)
{
  int rc = 0;

#ifdef WIN32
  m_thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)thread, this,0, 0);

  rc = (int)(m_thread) ? (0) : (-1);
#else
  rc = pthread_create(&m_thread, 0, (void*(*)(void*))thread, this);
#endif

  if(rc)
    return false;

  priority(pr);

  return true;
}

void kgmThread::kill()
{
#ifdef WIN32
  TerminateThread(m_thread, NULL);
#elif defined(ANDROID)
  pthread_kill(m_thread, 9);
#else
  pthread_cancel(m_thread);
#endif 

  m_thread = null;
}

void kgmThread::join()
{
#ifdef WIN32 
#else
  pthread_join(m_thread, NULL);
#endif 

  m_thread = 0;
}

void kgmThread::priority(Priority prio)
{
  if(!m_thread)
    return;

#ifdef ANDROID
#define SCHED_BATCH SCHED_NORMAL
#define SCHED_IDLE  SCHED_NORMAL
#endif


#ifdef WIN32
  int policy = THREAD_PRIORITY_NORMAL;

  switch(prio)
  {
  case PrNormal:
    policy = THREAD_PRIORITY_NORMAL;
    break;
  case PrLow:
    policy = THREAD_PRIORITY_BELOW_NORMAL;
    break;
  case PrIdle:
    policy = THREAD_PRIORITY_IDLE;
    break;
  case PrHight:
    policy = THREAD_PRIORITY_ABOVE_NORMAL;
    break;
  case PrSuper:
    policy = THREAD_PRIORITY_HIGHEST;
    break;
  default:
    policy = THREAD_PRIORITY_NORMAL;
    break;
  }

  SetThreadPriority(m_thread, policy);
#else
  int policy = SCHED_OTHER;
  sched_param param{0};

  switch(prio)
  {
  case PrNormal:
    policy = SCHED_OTHER;
    break;
  case PrLow:
    policy = SCHED_BATCH;
    break;
  case PrIdle:
    policy = SCHED_IDLE;
    break;
  case PrHight:
    policy = SCHED_RR;
    param.sched_priority = 20;
    break;
  case PrSuper:
    policy = SCHED_RR;
    param.sched_priority = 80;
    break;
  default:
    policy = SCHED_OTHER;
    break;
  }

  pthread_setschedparam(m_thread, policy, &param);
#endif
}

kgmThread::Mutex kgmThread::mutex()
{
#ifdef WIN32
  Mutex mutex = (Mutex)malloc(sizeof(CRITICAL_SECTION));

  if(mutex)
  {
    InitializeCriticalSection(mutex);

    return mutex;
  }

#else
  Mutex mutex = (Mutex)malloc(sizeof(pthread_mutex_t));

  if(mutex)
  {
    *mutex = PTHREAD_MUTEX_INITIALIZER;

    return mutex;
  }
#endif

  return null;
}

void  kgmThread::mxfree(kgmThread::Mutex m)
{
  if(m)
  {
    unlock(m);

#ifdef WIN32
    DeleteCriticalSection(m);

    free(m);
#else
    pthread_mutex_destroy((pthread_mutex_t*)m);

    free(m);
#endif
  }
}

void  kgmThread::lock(kgmThread::Mutex m)
{
  if(m)
  {
#ifdef WIN32
    EnterCriticalSection(m);
#else
    pthread_mutex_lock(m);
#endif
  }
}

void  kgmThread::unlock(kgmThread::Mutex m)
{
  if(m)
  {
#ifdef WIN32
    LeaveCriticalSection(m);
#else
    pthread_mutex_unlock(m);
#endif
  }
}

bool  kgmThread::lockable(Mutex m)
{
  if(m)
  {
#ifdef WIN32
    if(!TryEnterCriticalSection(m))
    {
      LeaveCriticalSection(m);

      return true;
    }
#else
    if(!pthread_mutex_trylock(m))
    {
      pthread_mutex_unlock(m);

      return true;
    }
#endif
  }

  return false;
}

kgmThread::TID  kgmThread::id()
{
  TID tid = 0;

#ifdef WIN32
  tid = GetCurrentThreadId();
#else
  tid = (kgmThread::TID)pthread_self();
#endif

  return tid;
}
