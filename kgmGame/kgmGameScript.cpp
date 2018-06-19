#include "kgmGameScript.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "kgmGameTools.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmScript/kgmLuaScript.h"
#include "../kgmGraphics/kgmGui.h"

kgmGameScript::kgmGameScript(kgmIGame* g)
{
  game = g;

  handler = new kgmLuaScript(g->getResources());

  handler->load("main");

  handler->set("kgmLog",   kgmGameScript::kgmLog);

  handler->set("kgmGameExit",   kgmGameScript::kgmGameExit);
  handler->set("kgmGamePlay",   kgmGameScript::kgmGamePlay);
  handler->set("kgmGamePause",  kgmGameScript::kgmGamePause);
  handler->set("kgmGameState",  kgmGameScript::kgmGameState);
}

kgmGameScript::~kgmGameScript()
{
  delete handler;
}

void kgmGameScript::init()
{
  handler->call("main_init", "");
}

void kgmGameScript::free()
{
  handler->call("main_free", "");
}

void kgmGameScript::update()
{
  handler->call("main_update", "i", game->timeUpdate());
}

void kgmGameScript::kgmLog(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  s8* log = null;

  game->getScript()->args("s", &log);

  kgm_log_print(log);
}

void kgmGameScript::kgmGameExit(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  if(game->gState() != kgmIGame::State_Quit)
    game->gQuit();
}

void kgmGameScript::kgmGamePlay(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  game->gSwitch(kgmIGame::State_Play);
}

void kgmGameScript::kgmGamePause(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  game->gSwitch(kgmIGame::State_Pause);
}

void kgmGameScript::kgmGameState(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  state = game->gState();

  game->getScript()->resl("i", state);
}

void kgmGameScript::kgmGui(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  s8* sid = null;

  void* gui = null;

  game->getScript()->args("s", &sid);

  if (sid)
  {
    kgmMemory<u8> mem;
    game->getResources()->getFile(sid, mem);

    kgmXml xml(mem);
    gui = kgmGameTools::genGui(xml);
  }

  game->getScript()->resl("p", gui);
}

void kgmGameScript::kgmGuiShow(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  state = game->gState();

  game->getScript()->resl("i", state);
}
