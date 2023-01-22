#include "utils.h"
#include "../kgmGame/kgmGameApp.h"

void updateFrame()
{
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

  kgm_darwin_init(500, 500);
  
  __kgmLog("StartingRunning app");
  __kgmRunApp();

  kgm_darwin_quit();
  
  __kgmLog("Finishing app");
  __kgmExitApp();
}
