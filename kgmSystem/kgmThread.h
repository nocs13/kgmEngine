// cThread.h: interface for the cThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifdef WIN32
 #include <windows.h>
 #include <process.h>
#elif defined LINUX
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
#endif


class kgmThread{
public:
#ifdef WIN32
 void* m_thread;
#endif 
#ifdef LINUX
 pthread_t m_thread;
#endif 
public:
 kgmThread();
 ~kgmThread();

 bool exec();
 void exit();

protected: 
 virtual void run() = 0;
private:
 static void thread(kgmThread *p);
};

