#pragma once

#include "kgmObject.h"
#include "kgmString.h"
#include "kgmInterface.h"

class kgmISettings: public kgmInterface
{
public:
  virtual void load() = 0;
  virtual void save() = 0;

  virtual kgmString get(s8*) = 0;
  virtual void      set(s8*, s8*) = 0;
};
