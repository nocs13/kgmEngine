#ifndef KGMGAMEMESSENGER_H
#define KGMGAMEMESSENGER_H

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmEvent.h"
#include "kgmIGame.h"

class kgmGameMessenger: public kgmObject, public kgmIGame::Messenger
{
  KGM_OBJECT(kgmGameMessenger);

  kgmIGame* game = null;

  kgmList<kgmIGame::Listener*> list;

public:
  kgmGameMessenger(kgmIGame*);

  void add(kgmIGame::Listener*);
  void remove(kgmIGame::Listener*);

  void onEvent(kgmEvent::Event* e);
};

#endif // KGMGAMEMESSENGER_H
