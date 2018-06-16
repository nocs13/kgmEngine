#include "kgmGameScript.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "../kgmScript/kgmLuaScript.h"

kgmGameScript::kgmGameScript(kgmIGame* g)
{
  game = g;

  handler = new kgmLuaScript(g->getResources());

  handler->load("main");

  handler->set("kgmGameExit",   kgmGameScript::kgmGameExit);
  handler->set("kgmGamePlay",   kgmGameScript::kgmGamePlay);
  handler->set("kgmGamePause",  kgmGameScript::kgmGamePause);
  handler->set("kgmGameState",  kgmGameScript::kgmGameState);
}

kgmGameScript::~kgmGameScript()
{
  delete handler;
}

void kgmGameScript::update()
{
  handler->call("main_update", "i", game->timeUpdate());
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
