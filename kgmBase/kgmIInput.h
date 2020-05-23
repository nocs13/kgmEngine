#pragma once
#include "kgmTypes.h"
#include "kgmInterface.h"

class kgmIInput
{
public:
  virtual s32 keyState(u8 key) = 0;
};
