#pragma once
#include "kgmTypes.h"
#include "kgmInterface.h"

class kgmIApp: public kgmInterface
{
public:
  virtual s32 main(s32, s8**) = 0;
  virtual void abort() = 0;
};
