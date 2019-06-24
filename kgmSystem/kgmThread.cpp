#include "kgmThread.h"
#include "kgmSystem.h"

void kgmThread::thread(kgmThread *p)
{
  if(!p)
    return;

#ifdef WIN32
#elif defined (ANDROID)
#else
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif

  p->run();

  kgmThread::exit(p->m_result);
}

void kgmThread::thread_1(kgmThread::Thread_Member_Function* p)
{
  if(!p)
    return;

  //(*p)();
}

kgmThread::Thread kgmThread::thread_create(Thread_Function fn, void* obj, kgmThread::Priority pr)
{
  Thread thread;

#ifdef WIN32
  thread = (Thread) CreateThread(0, 0, (LPTHREAD_START_ROUTINE) fn, obj, 0, 0);
#else
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

 int  rc = pthread_create(&thread, &attr, (void*(*)(void*))fn, obj);

 if (rc)
   thread = (Thread) null;

  pthread_attr_destroy(&attr);
#endif

  kgmThread::thread_priority(thread, pr);

  return thread;
}

kgmThread::Thread thread_create(kgmThread::Thread_Member_Function fn, kgmThread::Priority pr)
{
  kgmThread::Thread thread;

#ifdef WIN32
  thread = (kgmThread::Thread) CreateThread(0, 0, (LPTHREAD_START_ROUTINE) kgmThread::thread_1, &fn, 0, 0);
#else
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

 int  rc = pthread_create(&thread, &attr, (void*(*)(void*))kgmThread::thread_1, &fn);

 if (rc)
   thread = (kgmThread::Thread) null;

  pthread_attr_destroy(&attr);
#endif

  kgmThread::thread_priority(thread, pr);

  return thread;
}

bool kgmThread::thread_kill(kgmThread::Thread th)
{
#ifdef WIN32
  TerminateThread(th, -1);
#elif defined(ANDROID)
  pthread_kill(th, 9);
#else
  pthread_cancel(th);
#endif

  return true;
}

bool kgmThread::thread_join(kgmThread::Thread th)
{
#ifdef WIN32
  WaitForSingleObject(th, INFINITE);
#else
  pthread_join(th, NULL);
#endif

  return true;
}

bool kgmThread::thread_active(kgmThread::Thread th)
{
#ifdef WIN32
  DWORD id = GetThreadId(th);

  if (id && !GetLastError())
    return true;
#else
  if(pthread_kill(th, 0) == 0)
    return true;
#endif

  return false;
}

bool kgmThread::thread_priority(kgmThread::Thread th, kgmThread::Priority pr)
{
  if(!th)
    return false;

#ifdef WIN32

  int policy = THREAD_PRIORITY_NORMAL;

  switch(pr)
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

  SetThreadPriority(th, policy);

#else

  int policy = SCHED_OTHER;

  sched_param param{0};

  switch(pr)
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

  pthread_setschedparam(th, policy, &param);
#endif

  return true;
}

kgmThread::Mutex kgmThread::mutex_create()
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
    pthread_mutex_init(mutex, NULL);

    return mutex;
  }
#endif

  return null;
}

void  kgmThread::mutex_free(kgmThread::Mutex m)
{
  if(m)
  {
    mutex_unlock(m);

#ifdef WIN32
    DeleteCriticalSection(m);

    free(m);
#else
    pthread_mutex_destroy((pthread_mutex_t*)m);

    free(m);
#endif
  }
}

void  kgmThread::mutex_lock(kgmThread::Mutex m)
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

void  kgmThread::mutex_unlock(kgmThread::Mutex m)
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

bool  kgmThread::mutex_lockable(Mutex m)
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
  tid = (kgmThread::TID) pthread_self();
#endif

  return tid;
}

void kgmThread::exit(s32 res)
{
#ifdef WIN32
  ExitThread((DWORD)res);
#else
  long r = res;
  pthread_exit((void*) r);
#endif
}

void kgmThread::sleep(u32 ms)
{
#ifdef WIN32
  Sleep(ms);
#else
  usleep(ms * 1000);
  /*struct timespec req = {0, ms * 1000000};
  int res = nanosleep(&req, NULL);

  if(res != 0)
  {

  }*/
#endif
}


kgmThread::kgmThread()
{
  m_thread = 0;
  m_result = 0;

  m_object   = null;
  m_callback = null;
}

kgmThread::~kgmThread()
{
  thread_kill(m_thread);
}

bool kgmThread::start()
{
  m_thread = thread_create((Thread_Function) thread, this);

  if (!m_thread)
    return false;

  return true;
}

bool kgmThread::active()
{
  if(!m_thread)
    return false;

  return thread_active(m_thread);
}

void kgmThread::kill()
{
  thread_kill(m_thread);

  m_thread = null;
}

void kgmThread::join()
{
  thread_join(m_thread);

  m_thread = null;
}

void kgmThread::priority(kgmThread::Priority pr)
{
  if(!m_thread)
    return;

  thread_priority(m_thread, pr);
}

void kgmThread::run()
{
  if(m_callback)
    m_result = m_callback(m_object);
}
