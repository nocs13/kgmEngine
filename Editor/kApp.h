#ifndef KAPP_H
#define KAPP_H
#include "../../kgmGame/kgmGameApp.h"
#include "kEditor.h"

class kApp : public kgmGameApp
{
  kEditor* editor = null;
  bool failed = false;

public:
  kApp();
  ~kApp();

  void main();

  void gameInit();
  void gameLoop();
  void gameFree();
};
#endif // KAPP_H
