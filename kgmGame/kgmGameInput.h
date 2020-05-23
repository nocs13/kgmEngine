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

public:
  kgmGameInput(kgmIGame* g, u8* keys, s32 len);
  ~kgmGameInput();

  void clear();

  s32 keyState(u8 key);
};

#endif // KGMGAMEINPUT_H
