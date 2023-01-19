#include "utils.h"

int main() {
  __kgmLog("Starting app");
  __kgmStartApp();
  
  void* w = __kgmOpenMainWindow("kgm", 0, 0, 500, 500);
  int res = __kgmInitMetal(w);

  if (res != 0) {
    __kgmLog("Failed init metal.");
  }
  
  __kgmLog("StartingRunning app");
  __kgmRunApp();
  __kgmLog("Finishing app");
  __kgmExitApp();
}
