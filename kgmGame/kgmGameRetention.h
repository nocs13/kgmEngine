#ifndef KGMGAMERETENTION_H
#define KGMGAMERETENTION_H

#include "../kgmBase/kgmObject.h"

// Provide ability save game/level/player data.

class kgmGameRetention : public kgmObject
{
public:
  kgmGameRetention();

  bool serialize(bool in = true);

  bool load();
  bool keep();

  void set(kgmString key, kgmString val);
  bool get(kgmString key, kgmString& val);
};

#endif // KGMGAMERETENTION_H
