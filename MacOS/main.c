#include "utils.h"
#include "../kgmGame/kgmGameBase.h"

kgmIGame* game = NULL;

void updateFrame()
{
  if (game)
    game->getWindow()->onIdle();
  
  printf("updateFrame.\n");
}

int main() {
  __kgmLog("Starting app");
  __kgmStartApp();
  
  void* w = __kgmOpenMainWindow("kgm", 0, 0, 500, 500);
  int res = __kgmInitMetal(w);

  if (res != 0) {
    __kgmLog("Failed init metal.");
  }

  game = new kgmGameBase();

  game->getWindow()->setRect(0, 0, 500, 500);
  
  __kgmLog("StartingRunning app");
  __kgmRunApp();

  delete game;
  
  __kgmLog("Finishing app");
  __kgmExitApp();
}
