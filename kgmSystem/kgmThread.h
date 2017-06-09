// cThread.h: interface for the cThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmObject.h"

#ifdef WIN32
 #include <windows.h>
 #include <winbase.h>
#else
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
 #include <sched.h>
 #include <signal.h>
#endif

class kgmThread: public kgmObject
{
  KGM_OBJECT(kgmThread);

public:
  typedef long  TID;

  typedef int (*Thread_Function)(void*);

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

  enum Flags
  {
    CtNone   = 0,
    CtDetach = 1L << 1
  };

private:
#ifdef WIN32
 void* m_thread;
#else
 pthread_t m_thread;
#endif

 void   *m_object;
 int   (*m_callback)(void*);

 s32  m_result;

public:
 kgmThread();
 kgmThread(int (*call)(void*), void* obj = null, uint sets = CtNone, uint pr = PrNormal);
 ~kgmThread();

 bool exec(uint sets = CtNone, uint pr = PrNormal);
 bool exec(int (*call)(void*), void* obj = null, uint sets = CtNone, uint pr = PrNormal);
 bool active();
 
 void kill();
 void join();
 void priority(uint);

 static Mutex mutex();
 static void  mxfree(Mutex);
 static void  lock(Mutex);
 static void  unlock(Mutex);
 static bool  lockable(Mutex);

 static TID   id();
 static void  exit(s32 res);
 static void  sleep(u32 ms);

protected:
 virtual void run();

private:
 static void thread(kgmThread *p);
};
