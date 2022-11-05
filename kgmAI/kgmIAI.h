#ifndef KGMIAI_H
#define KGMIAI_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmVariable.h"

class kgmIGame;
class kgmUnit;
class kgmSound;
class kgmAnimation;
struct Action;

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

  struct State
  {
    kgmString id;
    kgmString type;
    kgmString switchto;
    kgmString switchfrom;

    u32       priopity;
    u32       timeout;

    kgmSound*      sound     = null;
    kgmAnimation*  animation = null;
    u32           fstart, fend;

    Action action;
  };

  struct Input
  {
    u32        input;
    u32        input1;
    u32        input2;
    u32        status;

    kgmString  state;
  };

  struct UnitType
  {
    kgmString type;

    kgmList<State*>     m_states;
    kgmList<Input>      m_inputs;
  };

  struct Unit
  {
    u32       stime;
    State*    state;
    UnitType* type;

    kgmUnit* unit;
  };

public:

  kgmAI()
  {

  }

  ~kgmAI()
  {

  }

  void update();
};

#endif // KGMIAI_H
