#ifndef KGMGAMEMESSENGER_H
#define KGMGAMEMESSENGER_H

#include "../kgmBase/kgmObject.h"
#include "kgmIGame.h"

class kgmGameMessenger: public kgmObject, public kgmIGame::Messenger
{
  KGM_OBJECT(kgmGameMessenger);

  kgmIGame* game = nullptr;

public:
  kgmGameMessenger(kgmIGame*);

  void add(kgmIGame::Listener*);
  void remove(kgmIGame::Listener*);
};

#endif // KGMGAMEMESSENGER_H
