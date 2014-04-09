#ifndef KGMGUITEXT_H
#define KGMGUITEXT_H

#include "kgmGui.h"

class kgmGuiText : public kgmGui
{
  KGM_OBJECT(kgmGuiText);

private:
  bool editable;
  bool shift;
  u32  index;

public:
  kgmGuiText();
  kgmGuiText(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  virtual ~kgmGuiText();

  void setEditable(bool e);
  void dropCursor();
  bool isReadOnly();
  u32  getCursor();

  static char toAnsii(bool shift, u16 key);

protected:
  void onMsLeftDown(int k, int x, int y);
  void onKeyDown(int k);
  void onKeyUp(int k);
};

#endif // KGMGUITEXT_H
