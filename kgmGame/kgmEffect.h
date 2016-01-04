#pragma once
#include "kgmUnit.h"

class kgmEffect : public kgmUnit
{
  KGM_UNIT(kgmEffect);

public:
  kgmEffect(kgmIGame *g);

  void init();
  void exit();
};

