#include "kgmThread.h"

void kgmThread::thread(kgmThread *p){
 if(!p)
   return;
 p->run();
}

kgmThread::kgmThread(){
 m_thread = 0;
}

kgmThread::~kgmThread(){
 if(m_thread)
  exit();
}

bool kgmThread::exec(){
 int rc = 0;
#ifdef WIN32 
 m_thread = (void*)_beginthread((void (*)(void*) )thread, 0, this);
#endif 
#ifdef LINUX 
 rc = pthread_create(&m_thread, 0, (void*(*)(void*))thread, this);
#endif 
 if(rc)
  return false;
 return true;
}

void kgmThread::exit(){
#ifdef WIN32 
 _endthread();
#endif 
#ifdef LINUX
 pthread_cancel(m_thread);
#endif 
 m_thread = 0;
}

