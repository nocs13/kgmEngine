#include "kApp.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmSystem/kgmSystem.h"

kApp::kApp()
{
  m_game = null;
  editor = null;
}

kApp::~kApp()
{
}

void kApp::gameInit()
{
}

void kApp::gameLoop()
{
}

void kApp::gameFree()
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