#include "kApp.h"
#include "../../kgmBase/kgmLog.h"

kApp::kApp()
{
}

void kApp::main()
{
  kgm_log() << "Start\n";

  editor = new kEditor();

  editor->loop();

  delete editor;
}

kApp theApp;
