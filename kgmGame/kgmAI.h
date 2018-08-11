#ifndef KGMIAI_H
#define KGMIAI_H

#include "../kgmBase/kgmObject.h"

class kgmAI: public kgmObject
{
  KGM_OBJECT(kgmAI);

  typedef void (*ActionCallback)(kgmIGame*, kgmAI*, Action*);

  struct Action
  {
    kgmString id;
    u32       time;
    u32       state;

    kgmList<kgmVariable> variables;

    ActionCallback callback = null;
  };

public:

  kgmAI()
  {

  }

  ~kgmAI()
  {

  }
};

#endif // KGMIAI_H
