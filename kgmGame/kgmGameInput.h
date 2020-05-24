#ifndef KGMGAMEINPUT_H
#define KGMGAMEINPUT_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmIInput.h"

class kgmIGame;

class kgmGameInput: public kgmObject, public kgmIInput
{
  KGM_OBJECT(kgmGameInput);

  kgmIGame* m_game;

  u8* m_keys;

  s32 m_length;

  s32 m_points[2][3];

public:
  kgmGameInput(kgmIGame* g, u8* keys, s32 len);
  ~kgmGameInput();

  s32 keyState(u8 key);
  Point pointState(s32 id);

  void setX(s32 id, s32 x);
  void setY(s32 id, s32 y);
  void setZ(s32 id, s32 z);
};

#endif // KGMGAMEINPUT_H
