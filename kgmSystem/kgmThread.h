// cThread.h: interface for the cThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmFunction.h"

#ifdef WIN32
 // Need update _WIN32_WINNT to 0x0600 in _mingw.h
 #include <windows.h>
 #include <winbase.h>
 //#include <synchapi.h>
 //typedef PVOID CONDITION_VARIABLE, *PCONDITION_VARIABLE;
#else
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
 #include <sched.h>
 #include <signal.h>
#endif

class kgmThread: public kgmObject
{
  KGM_OBJECT(kgmThread)

public:

#ifdef WIN32
 typedef void* Thread;
#else
 typedef pthread_t Thread;
#endif

  typedef long  TID;

  typedef int (*Thread_Function)(void*);

  typedef kgmFunction<void(void*)> Thread_Member_Function;

#ifdef WIN32
  typedef CRITICAL_SECTION*   Mutex;
  typedef CONDITION_VARIABLE* Condition;
#else
  typedef pthread_mutex_t* Mutex;
  typedef pthread_cond_t*  Condition;
#endif

  enum Priority
  {
    PrNormal = 0,
    PrLow,
    PrHight
  };

  static Thread thread_create(Thread_Function fn, void* obj = null, Priority pr = PrNormal);
  static Thread thread_create(Thread_Member_Function fn, void* obj = null, Priority pr = PrNormal);
  static bool   thread_kill(Thread th);
  static bool   thread_join(Thread th);
  static bool   thread_active(Thread th);
  static bool   thread_priority(Thread th, Priority pr);

  static Mutex mutex_create(bool recursive = false);
  static void  mutex_free(Mutex);
  static void  mutex_lock(Mutex);
  static void  mutex_unlock(Mutex);
  static bool  mutex_locked(Mutex);
  static bool  mutex_lockable(Mutex);

  static Condition condition_create();
  static void      condition_free(Condition);
  static bool      condition_wait(Condition, Mutex);
  static bool      condition_wait_time(Condition, Mutex, s32 ms);
  static bool      condition_signal(Condition);

  static TID   id();
  static void  exit(s32 res);
  static void  sleep(u32 ms);

private:
 Thread m_thread;
 Mutex  m_mutex;

 void   *m_object;
 int   (*m_callback)(void*);

 s32  m_result;

public:
 kgmThread();
 kgmThread(const kgmThread&) = delete;
 kgmThread & operator = (const kgmThread &) = delete;
 ~kgmThread();

 bool start();
 bool active();
 void kill();
 void join();
 void priority(Priority);

 void lock();
 void unlock();

protected:
 virtual void run();

private:
 static void thread(kgmThread *p);

public:
 static void thread_1(Thread_Member_Function& p, void* obj = null);
};
