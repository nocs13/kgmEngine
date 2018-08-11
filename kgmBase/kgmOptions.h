#ifndef KGMOPTIONS_H
#define KGMOPTIONS_H

#include "kgmString.h"
#include "kgmTab.h"

class kgmOptions
{
  kgmTab<kgmString, kgmString> options;

public:
  kgmOptions();

  kgmString get(kgmString key);
  void      set(kgmString key, kgmString value);
};

#endif // KGMOPTIONS_H
