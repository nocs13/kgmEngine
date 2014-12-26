#ifndef KGMGUITEXT_H
#define KGMGUITEXT_H

#include "kgmGui.h"
#include "../kgmBase/kgmCallback.h"

class kgmGuiText : public kgmGui
{
  KGM_OBJECT(kgmGuiText);
public:

  typedef kgmCallback<void, kgmObject*, kgmString> ChangeEventCallback;

private:
  bool editable;
  bool numeric;
  bool shift;
  u32  index, place;     //cursor index in text and place in view
  u32  fwidth, fheight;  //character width and height

  ChangeEventCallback callback;

public:
  kgmGuiText();
  kgmGuiText(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  virtual ~kgmGuiText();

  void setEditable(bool e);
  void setNumeric(bool e);
  void dropCursor();
  bool isReadOnly();
  bool isNumeric();
  u32  getCursor();
  u32  getIndex();

  void setChangeEventCallback(ChangeEventCallback call)
  {
    callback = call;
  }

  void setCharResolution(u32 w, u32 h)
  {
    fwidth = w;
    fheight = h;
  }

  u32 getCharWidth()
  {
    return fwidth;
  }

  u32 getCharHeight()
  {
    return fheight;
  }

  static char toAnsii(bool shift, u16 key);

protected:
  void onMsLeftDown(int k, int x, int y);
  void onKeyDown(int k);
  void onKeyUp(int k);

private:
  void delLeft();
  void delRight();
  void moveLeft();
  void moveRight();
};

#endif // KGMGUITEXT_H
