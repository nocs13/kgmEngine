#pragma once

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "kgmThread.h"

class kgmProcess: public kgmObject
{
  class Thread : public kgmThread
  {
    kgmString m_command;
    bool      m_wait;

  public:
    Thread(bool wait = false)
    {
      m_wait = wait;
    }

    void command(kgmString cmd)
    {
      m_command = cmd;

      exec();

      if(m_wait)
        join();
    }

    void run()
    {
#ifdef WIN32
      system(m_command);
#else
      system(m_command);
#endif
    }
  };

  Thread    m_thread;
  kgmString m_cmd;
public:

  kgmProcess();
};

