#ifndef KGMTHREADPOOL_H
#define KGMTHREADPOOL_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmArray.h"
#include "../kgmBase/kgmPointer.h"
#include "../kgmBase/kgmFunction.h"
#include "kgmThread.h"

class kgmThread;

class kgmThreadPool: public kgmObject
{
  KGM_OBJECT(kgmThreadPool);

private:
  struct Worker
  {
    kgmThread thread;


    Worker()
    {

    }

    ~Worker()
    {

    }
  };

  kgmArray<kgm_uptr<Worker>> workers;

public:
  kgmThreadPool(u8 count = 4);
};

#endif // KGMTHREADPOOL_H
