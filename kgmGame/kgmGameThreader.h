#pragma once

#include "../kgmBase/kgmObject.h"
#include "../kgmSystem/kgmThread.h"

class kgmGameThreader: public kgmObject
{
  KGM_OBJECT(kgmGameThreader);

public:
  typedef void (*THREADER_FUNCTION)(void*);

private:

#define MAX_THREADERS 128

  struct Threader
  {
    THREADER_FUNCTION funtion;
    void*             object;
  };

  bool m_ready  = false;
  bool m_locked = false;
  bool m_active = true;

  u32  m_count = 0;

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
  void ready();
  bool finish();

  static int threader(void*);
};
