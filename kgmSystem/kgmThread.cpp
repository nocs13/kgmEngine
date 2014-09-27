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
#else
  pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

  *mutex = PTHREAD_MUTEX_INITIALIZER;

  if(mutex)
    return mutex;
#endif

  return null;
}

void  kgmThread::mxfree(kgmThread::Mutex m)
{
  if(m)
  {
#ifdef WIN32
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
#else
    int rc = pthread_mutex_lock((pthread_mutex_t*)m);
#endif
  }
}

void  kgmThread::unlock(kgmThread::Mutex m)
{
  if(m)
  {
#ifdef WIN32
#else
    int rc = pthread_mutex_unlock((pthread_mutex_t*)m);
#endif
  }
}

bool  kgmThread::lockable(Mutex m)
{
  if(m)
  {
#ifdef WIN32
#else
    if(!pthread_mutex_trylock((pthread_mutex_t*)m))
    {
      pthread_mutex_unlock((pthread_mutex_t*)m);

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
#else
  tid = (kgmThread::TID)pthread_self();
#endif

  return tid;
}
