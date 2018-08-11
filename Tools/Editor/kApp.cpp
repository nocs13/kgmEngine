#include "kApp.h"
#include "../../kgmBase/kgmLog.h"

kApp::kApp()
{
  m_game = null;
  editor = null;
}

void kApp::main()
{
  kgm_log() << "Start\n";

  editor = new kEditor();
  m_game = editor;

  editor->loop();

  delete editor;
}

kApp theApp;
