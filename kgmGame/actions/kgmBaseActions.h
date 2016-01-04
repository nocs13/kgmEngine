#pragma once

#include "../kgmIGame.h"
#include "../kgmActor.h"

class kgmBaseActions
{
public:
  static kgmVariable find(kgmList<kgmVariable>&, kgmString);
  static bool get(kgmList<kgmVariable>&, kgmString, int&);
  static bool get(kgmList<kgmVariable>&, kgmString, float&);
  static bool get(kgmList<kgmVariable>&, kgmString, kgmString&);

  static void register_actions();

  static void nothing(kgmIGame*, kgmActor*, kgmActor::Action*);
  static void accelerate(kgmIGame*, kgmActor*, kgmActor::Action*);
};
