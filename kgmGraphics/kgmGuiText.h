#ifndef KGMGUITEXT_H
#define KGMGUITEXT_H

#include "kgmGui.h"

class kgmGuiText : public kgmGui
{
  KGM_OBJECT(kgmGuiText);

public:
  kgmGuiText();
  kgmGuiText(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  virtual ~kgmGuiText();
};

#endif // KGMGUITEXT_H
