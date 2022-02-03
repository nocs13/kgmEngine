#pragma once

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "kgmThread.h"

#ifdef WIN32
#else
  #include <unistd.h>
#endif

class kgmProcess: public kgmObject
{
  size_t m_process;

  kgmString m_out;

  bool m_err;
public:
  kgmProcess();
  bool run(kgmString cmd, kgmString args);
  kgmString out();
};

