#pragma once
#include "kgmTypes.h"
#include "kgmInterface.h"

class kgmIApp: public kgmInterface
{
public:
  virtual s32  exec(s32, s8**) = 0;
};
