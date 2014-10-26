// cThread.h: interface for the cThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../kgmBase/kgmObject.h"

#ifdef WIN32
 #include <windows.h>
 #include <winbase.h>
#else
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
 #include <sched.h>
#endif

#ifdef WIN32
#else
#endif

class kgmThread: public kgmObject
{
  KGM_OBJECT(kgmThread);

public:
  typedef long  TID;

#ifdef WIN32
  typedef CRITICAL_SECTION* Mutex;
#else
  typedef pthread_mutex_t* Mutex;
#endif

  enum Priority
  {
    PrNormal = 0,
    PrLow,
    PrIdle,
    PrHight,
    PrSuper
  };

private:
#ifdef WIN32
 void* m_thread;
#else
 pthread_t m_thread;
#endif

 bool m_canselable;

public:
 kgmThread();
 ~kgmThread();

 bool exec(bool canselable = false, Priority pr = PrNormal);
 void kill();
 void join();
 void priority(Priority);

 static Mutex mutex();
 static void  mxfree(Mutex);
 static void  lock(Mutex);
 static void  unlock(Mutex);
 static bool  lockable(Mutex);

 static TID   id();
 static void  sleep(u32 ms);

protected:
 virtual void run() = 0;

private:
 static void thread(kgmThread *p);
};

class kgmInstThread: public kgmThread
{
  void* object;
  int (*callback)(void*);

public:
  kgmInstThread()
  {
    object   = null;
    callback = null;
  }

  kgmInstThread(void* obj, int (*call)(void*))
  {
    object   = obj;
    callback = call;

    start(obj, call);
  }

  void start(void* obj, int (*call)(void*))
  {
    object   = obj;
    callback = call;

    if(call && obj)
      exec();
  }

  virtual void run() final
  {
    if(object && callback)
      callback(object);
  }
};
