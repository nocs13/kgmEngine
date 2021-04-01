#include "kApp.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmSystem/kgmSystem.h"

kApp::kApp()
{
  m_game = null;
  editor = null;
}

void kApp::gameInit()
{
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);

    editor = new kEditor();

    m_game = static_cast<kgmIGame*> (editor);

    setMainWindow(editor);

    if (editor && !editor->gInit())
    {
      failed = true;
    }

    //wnd = new kgmWindow((kgmWindow*) null, "kgm", 0, 0, 800, 600, 24, false);
}

void kApp::gameLoop()
{
    if(editor != null && failed != true)
    {
      editor->loop();
    }
}

void kApp::gameFree()
{
    if(editor)
    {
      editor->release();

      m_game = editor = null;
    }
}

void kApp::main()
{
  kgm_log() << "Start\n";

  editor = new kEditor();
  m_game = editor;

  editor->loop();
}

kApp theApp;
