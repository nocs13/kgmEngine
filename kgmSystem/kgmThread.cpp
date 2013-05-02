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

kgmThread::Mutex kgmThread::mutex()
{
#ifdef WIN32
  //_endthread();
#else
  pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
  *mutex = PTHREAD_MUTEX_INITIALIZER;

  return mutex;
#endif
  return NULL;
}

void  kgmThread::free(kgmThread::Mutex m)
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

