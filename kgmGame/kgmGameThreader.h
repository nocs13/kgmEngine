#pragma once

#include "../kgmBase/kgmObject.h"
#include "../kgmSystem/kgmThread.h"

class kgmGameThreader: public kgmObject
{
  KGM_OBJECT(kgmGameThreader);

public:
  typedef void (*THREADER_FUNCTION)(void*);

private:

#define MAX_THREADS 8
#define MAX_WORKERS 8

  struct Worker
  {
    THREADER_FUNCTION funtion;
    void*             object;
  };

  struct Thread
  {
    kgmThread::Thread thread;
    kgmThread::Mutex  mutex;

    Worker workers[MAX_WORKERS];
    u32    count;

    kgmGameThreader* gt;
  };

  bool m_active = true;

  Thread m_threads[MAX_THREADS];

public:
  kgmGameThreader();
  ~kgmGameThreader();

  bool add(THREADER_FUNCTION fn, void* obj);
  bool remove(THREADER_FUNCTION fn);

  bool finish();

  static int threader(Thread*);
};
