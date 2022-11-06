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

class kgmIAI
{
  /*
  typedef void (*ActionCallback)(kgmIGame*, kgmIAI*, Action*);

  struct Action
  {
    kgmString id;
    u32       time;
    u32       state;

    kgmList<kgmVariable> variables;

    ActionCallback callback = null;
  };
  */
 public:
  struct State
  {
    kgmString id;
    kgmString action;
    kgmString switchto;
    kgmString switchfrom;

    u32       priopity;
    u32       timeout;

    kgmSound*      sound     = null;
    kgmAnimation*  animation = null;
    u32           fstart, fend;
  };

  struct Input
  {
    u32        input;
    u32        input1;
    u32        input2;
    u32        status;

    kgmString  state;
  };

protected:
  struct UnitType
  {
    kgmString type;

    kgmList<State> m_states;
    kgmList<Input> m_inputs;
  };

  struct Unit
  {
    u32       stime;
    State*    state;
    UnitType* type;

    kgmUnit* unit;
  };

public:

  virtual void update() = 0;
  virtual bool addType(kgmString) = 0;
  virtual bool addState(kgmString, State) = 0;
  virtual bool addInput(kgmString, Input) = 0;
  virtual bool addUnit(kgmUnit*) = 0;
};

#endif // KGMIAI_H
