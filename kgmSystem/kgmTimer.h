#ifndef KGMTIMER_H
#define KGMTIMER_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "kgmThread.h"
#include "kgmTime.h"

class kgmTimer : public kgmObject
{
  KGM_OBJECT(kgmTimer);

public:
  typedef void (*Function)(void*);

private:
  struct Chunk
  {
    u32      delay;
    u32      start;
    Function fn;
    void*    ob;
  };

  friend struct Thread;

  struct Thread: public kgmThread
  {
    kgmTimer* timer;
    bool      valid;

    Thread(kgmTimer* t);
    void run();
    void stop();
  };

  Thread* thread;

  kgmList<Chunk> list;

  kgmTime time;

public:
  kgmTimer();
  kgmTimer(const kgmTimer&) = delete;
  kgmTimer & operator = (const kgmTimer &) = delete;
  ~kgmTimer();

  bool add(Function fn, void* ob, u32 dy);
  void remove(Function fn);
};

#endif
