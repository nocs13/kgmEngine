#ifndef KINPUT_H
#define KINPUT_H

#include "../../kgmGame/objects/kgmObjects.h"
#include "../../kgmGame/objects/kgmSnInputListener.h"

class kInput: public kgmSnInputListener
{
  KGM_OBJECT(kInput);

  kgmIGame* game;

  u8 keys[128];

public:
  static kgmUnit* New(kgmIGame* g){ return new kInput(g); }

  kInput(kgmIGame* g)
    :kgmSnInputListener(g)
  {
    game = g;

    memset(keys, 0, 128);
  }

  void input(u32 btn, int state)
  {
    switch(btn)
    {
    case gbtn_a:
      if(state && keys[gbtn_x])
      {
      }
      else if(state && keys[gbtn_z])
      {
      }
      break;
    }

    keys[btn] = (u8)state;
  }
};

#endif // KINPUT_H
