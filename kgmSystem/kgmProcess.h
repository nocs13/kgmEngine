#pragma once

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "kgmThread.h"

class kgmProcess: public kgmObject
{
  size_t m_process;

public:
  kgmProcess();
  bool run(kgmString cmd, kgmString args);
  bool wait();
};

