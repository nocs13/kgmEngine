#ifndef KINPUT_H
#define KINPUT_H

#include "../../kgmGame/actors/kgmActors.h"

class kInput: public kgmInputListen
{
  KGM_OBJECT(kInput);

  kgmIGame* game;

public:
  kInput(kgmIGame* g)
  {
    game = g;
  }

  void input(u32 btn, int state)
  {
    switch(btn)
    {
    case 0:
      break;
    }
  }
};

#endif // KINPUT_H
