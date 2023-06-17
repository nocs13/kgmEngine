#pragma once
#include "kgmTypes.h"
#include "kgmInterface.h"

class kgmIInput: public kgmInterface
{
public:
  struct Point { s32 x, y, z; };

public:
  virtual s32 keyState(u8 key) = 0;
  virtual Point pointState(s32 id) = 0;
};
