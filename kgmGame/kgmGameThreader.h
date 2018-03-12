#pragma once

#include "../kgmBase/kgmObject.h"
#include "../kgmSystem/kgmThread.h"

class kgmGameThreader: public kgmObject
{
  KGM_OBJECT(kgmGameThreader);

public:
  typedef void (*THREADER_FUNCTION)(void*);

private:

#define MAX_THREADERS 32

  struct Threader
  {
    THREADER_FUNCTION funtion;
    void*             object;
  };

  bool m_locked = false;
  bool m_active = true;

  Threader m_threaders[MAX_THREADERS];

  kgmThread::Thread m_thread;

public:
  kgmGameThreader();
  ~kgmGameThreader();


  bool locked() const
  {
    return m_locked;
  }

  bool add(THREADER_FUNCTION fn, void* obj);
  bool remove(THREADER_FUNCTION fn);

  void ready();
  bool finish();

  static int threader(void*);
};
