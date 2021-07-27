#ifndef KGMGAMERETENTION_H
#define KGMGAMERETENTION_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmMap.h"
#include "../kgmBase/kgmEncryptor.h"

// Provide ability save game/level/player data.

class kgmGameRetention : public kgmObject
{
  KGM_OBJECT(kgmGameRetention)

  kgmMap<kgmString, kgmString> m_data;

  kgmString m_name;

  kgmEncryptor m_encryptor;

public:
  kgmGameRetention();

  void name(kgmString);
  void encryptor(s32 x, s32 y, s32 e, s32 d);
  bool serialize(bool in = true);

  bool load();
  bool keep();

  void      set(kgmString key, kgmString val);
  kgmString get(kgmString key);

private:
  kgmString getPath();
};

#endif // KGMGAMERETENTION_H
