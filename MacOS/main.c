#include "utils.h"

int main() {
  __kgmLog("Starting app");
  __kgmStartApp();
  
  void* w = __kgmOpenWindow("kgm", 0, 0, 500, 500);
  
  __kgmLog("StartingRunning app");
  __kgmRunApp();
  __kgmLog("Finishing app");
  __kgmExitApp();
}
