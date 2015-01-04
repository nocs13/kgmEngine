#pragma once
#include "kgmGameObject.h"

class kgmEffect : public kgmGameObject
{
  KGM_GO_OBJECT(kgmEffect);

public:
  kgmEffect(kgmIGame *g);

  void init();
};

