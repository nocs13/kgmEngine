#include "utils.h"
#include "../kgmGame/kgmGameBase.h"

kgmIGame* game = NULL;
void*     window = NULL;

void updateFrame() {
  if (game)
    game->getWindow()->onIdle();
  
//  printf("updateFrame.\n");
}

void* __kgmGetMainWindow() {
  return window;
}

int main() {
  __kgmLog("Starting kgm game app.\n");
  __kgmStartApp();
  
  void* w = window = __kgmOpenMainWindow("kgm", 0, 0, 500, 500);
  
  //__kgmLog("Initializing metal.");
  //int res = __kgmInitMetal(w);

  //if (res != 0) {
  //  __kgmLog("Failed init metal.");
  //}

  game = new kgmGameBase();

  game->getWindow()->setHandle(window);
  game->getWindow()->setRect(0, 0, 500, 500);

  game->gInit();
  
  __kgmLog("StartingRunning app");
  __kgmRunApp();

  delete game;
  
  __kgmLog("Finishing app");
  __kgmExitApp();
}
