#ifndef KGMGAMEINPUT_H
#define KGMGAMEINPUT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmIInput.h"

class kgmGameInput: public kgmObject, public kgmIInput
{
public:
  kgmGameInput();

  void clear();

  void keyUp(u8 key);
  void keyDown(u8 key);
  void movePoint(s16 x, s16 y, s16 z);
};

#endif // KGMGAMEINPUT_H
