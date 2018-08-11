#ifndef KGMCHARACTER_H
#define KGMCHARACTER_H

#include "../kgmActor.h"

class kgmGui;
class kgmIGame;

class kgmCharacter : public kgmActor
{
  KGM_OBJECT(kgmCharacter);

  float roll;
  float yaaw;
  float pich;
  float speed_idl;
  float speed_wlk;
  float speed_run;

  bool  gbtns[64];

  kgmGui *gui;

public:
  kgmCharacter(kgmIGame* g = null);

  void update(u32 ms);
  void action(kgmString&);
};

#endif // KGMCHARACTER_H
