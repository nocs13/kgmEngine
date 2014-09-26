// cThread.h: interface for the cThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../kgmBase/kgmObject.h"

#ifdef WIN32
 #include <windows.h>
 #include <process.h>
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
  typedef void* Mutex;

  enum Priority
  {
#ifdef WIN32
    PrLow = 0,
    PrNormal = 1,
    PrHight = 2
#else
    PrLow = SCHED_IDLE,
    PrNormal = SCHED_OTHER,
    PrHight = SCHED_BATCH
#endif
  };

private:
#ifdef WIN32
 void* m_thread;
#else
 pthread_t m_thread;
#endif
 
public:
 kgmThread();
 ~kgmThread();

 bool exec(bool canselable = false, Priority pr = PrNormal);
 void exit();
 void join();
 void priority(int);

 static Mutex mutex();
 static void  mxfree(Mutex);
 static void  lock(Mutex);
 static void  unlock(Mutex);
 static bool  lockable(Mutex);

 static TID   id();

protected:
 virtual void run() = 0;

private:
 static void thread(kgmThread *p);
};

