#pragma once
#include "kgmInterface.h"

class kgmIWindow: public kgmInterface
{
public:
  virtual void close() = 0;
  virtual void show(bool) = 0;
};
