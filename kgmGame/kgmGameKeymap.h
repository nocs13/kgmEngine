#ifndef KGMGAMEKEYMAP_H
#define KGMGAMEKEYMAP_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"

class kgmGameKeymap : public kgmObject
{
  KGM_OBJECT(kgmGameKeymap);

  struct Key { kgmString id; u8 key; };
  struct Btn { kgmString id; u8 btn; };

  kgmArray<kgmString> m_keys;
  kgmArray<kgmString> m_btns;

public:
  kgmGameKeymap();
};

#endif // KGMGAMEKEYMAP_H
