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

bool kgmThread::exec(){
 int rc = 0;

#ifdef WIN32 
 m_thread = (void*)_beginthread((void (*)(void*) )thread, 0, this);
#else 
 rc = pthread_create(&m_thread, 0, (void*(*)(void*))thread, this);
#endif 

 if(rc)
  return false;

 return true;
}

void kgmThread::exit()
{
#ifdef WIN32 
 _endthread();
#elif defined ANDROID
 pthread_kill(m_thread, 9);
#else
 pthread_cancel(m_thread);
#endif 
 m_thread = null;
}

void kgmThread::join()
{
#ifdef WIN32 
  //_endthread();
#else
  pthread_join(m_thread, NULL);
#endif 
 m_thread = 0;
}

void kgmThread::priority(int prio)
{
  if(!m_thread)
    return;

#ifdef WIN32
  //_endthread();
#else

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
#endif
 m_thread = 0;
}

kgmThread::Mutex kgmThread::mutex()
{
#ifdef WIN32
  //_endthread();
#else
  pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  *mutex = PTHREAD_MUTEX_INITIALIZER;

  return mutex;
#endif
  return NULL;
}

void  kgmThread::mxfree(kgmThread::Mutex m)
{
#ifdef WIN32
  //_endthread();
#else
  if(m)
  {
    pthread_mutex_destroy((pthread_mutex_t*)m);
    free(m);
  }
#endif
}

void  kgmThread::lock(kgmThread::Mutex m)
{
#ifdef WIN32
  //_endthread();
#else
  if(m)
  {
    int rc = pthread_mutex_lock((pthread_mutex_t*)m);
  }
#endif
}

void  kgmThread::unlock(kgmThread::Mutex m)
{
#ifdef WIN32
  //_endthread();
#else
  if(m)
  {
    int rc = pthread_mutex_unlock((pthread_mutex_t*)m);
  }
#endif
}

bool  kgmThread::lockable(Mutex m)
{
#ifdef WIN32
  //_endthread();
#else
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
#endif

  return false;
}

kgmThread::TID  kgmThread::idThread()
{
#ifdef WIN32
  //_endthread();
#else
  return pthread_self();
#endif

  return -1;
}

