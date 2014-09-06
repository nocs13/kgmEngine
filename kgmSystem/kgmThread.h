// cThread.h: interface for the cThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifdef WIN32
 #include <windows.h>
 #include <process.h>
 #include <pthread.h>
#else
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
 #include <sched.h>
#endif


class kgmThread
{
public:
typedef long  TID;
typedef void* Mutex;

#ifdef WIN32
 void* m_thread;
#else
 pthread_t m_thread;
#endif
 
public:
 kgmThread();
 ~kgmThread();

 bool exec();
 void exit();
 void join();
 void priority(int);

 static Mutex mutex();
 static void  mxfree(Mutex);
 static void  lock(Mutex);
 static void  unlock(Mutex);
 static bool  lockable(Mutex);

 static TID   idThread();

protected: 
 virtual void run() = 0;

private:
 static void thread(kgmThread *p);
};

