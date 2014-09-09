#include "kgmThread.h"
#include "kgmSystem.h"

void kgmThread::thread(kgmThread *p)
{
 if(!p)
   return;

 p->run();
}

kgmThread::kgmThread()
{
 m_thread = 0;
}

kgmThread::~kgmThread()
{
 if(m_thread)
  exit();
}

bool kgmThread::exec()
{
 int rc = 0;

 rc = pthread_create(&m_thread, 0, (void*(*)(void*))thread, this);

 if(rc)
  return false;

 return true;
}

void kgmThread::exit()
{
#ifdef ANDROID
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

void kgmThread::priority(int prio)
{
  if(!m_thread)
    return;

#ifdef ANDROID
#define SCHED_BATCH SCHED_NORMAL
#define SCHED_IDLE  SCHED_NORMAL
#endif

  int policy = SCHED_OTHER;
  sched_param param{0};

  switch(prio)
  {
  case -1:
      policy = SCHED_BATCH;
      break;
  case -2:
      policy = SCHED_IDLE;
      break;
  case 1:
      policy = SCHED_RR;
      param.sched_priority = 20;
      break;
  case 2:
      policy = SCHED_RR;
      param.sched_priority = 50;
      break;
  default:
      pthread_getschedparam(0, &policy, &param);
      break;
  }

  pthread_setschedparam(m_thread, policy, &param);

  m_thread = 0;
}

kgmThread::Mutex kgmThread::mutex()
{
  pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

  *mutex = PTHREAD_MUTEX_INITIALIZER;

  return mutex;
}

void  kgmThread::mxfree(kgmThread::Mutex m)
{
  if(m)
  {
    pthread_mutex_destroy((pthread_mutex_t*)m);

    free(m);
  }
}

void  kgmThread::lock(kgmThread::Mutex m)
{
  if(m)
  {
    int rc = pthread_mutex_lock((pthread_mutex_t*)m);
  }
}

void  kgmThread::unlock(kgmThread::Mutex m)
{
  if(m)
  {
    int rc = pthread_mutex_unlock((pthread_mutex_t*)m);
  }
}

bool  kgmThread::lockable(Mutex m)
{
  if(m)
  {
    if(!pthread_mutex_trylock((pthread_mutex_t*)m))
    {
      pthread_mutex_unlock((pthread_mutex_t*)m);

      return true;
    }
    else
    {
      return false;
    }
  }

  return false;
}

kgmThread::TID  kgmThread::idThread()
{
  return pthread_self();
}

