#include "kApp.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmSystem/kgmSystem.h"

kApp::kApp()
{
  editor = null;
}

kApp::~kApp()
{
}

void kApp::main()
{
}

int main(int argc, char** argv)
{
  kApp app;

  return  app.exec(argc, argv);
}
