#pragma once
#include "kgmUnit.h"

class kgmEffect : public kgmUnit
{
  KGM_OBJECT(kgmEffect);

public:
  kgmEffect(kgmIGame *g);

  void init();
  void exit();
};

