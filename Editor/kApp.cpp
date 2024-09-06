#include "kApp.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmSystem/kgmSystem.h"
#include "../kgmScript/kgmLuaScript.h"
#include "../kgmGame/kgmGameResources.h"

kApp::kApp()
{
  editor = null;
}

kApp::~kApp()
{
}

s32 kApp::exec(s32, s8**)
{
  fprintf(stderr, "XXX\n");

  kgmLuaScript* script = new kgmLuaScript();

  script->release();

  return 0;
}

int main(int argc, char** argv)
{
  kApp app;

  return  app.exec(argc, argv);
}
