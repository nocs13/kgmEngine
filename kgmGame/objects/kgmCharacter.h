#ifndef KGMCHARACTER_H
#define KGMCHARACTER_H

#include "../kgmActor.h"

class kgmGui;
class kgmIGame;

class kgmCharacter : public kgmActor
{
  KGM_GO_OBJECT(kgmCharacter);

  float roll;
  float yaaw;
  float pich;
  float speed_max;
  float speed_min;

  bool  gbtns[64];

  kgmGui *gui;

public:
  kgmCharacter(kgmIGame* g = null);

  void update(u32 ms);
};

#endif // KGMCHARACTER_H
