#include "kgmGameScript.h"
#include "kgmIGame.h"
#include "../kgmScript/kgmLuaScript.h"

kgmGameScript::kgmGameScript(kgmIGame* g)
{
  game = g;

  handler = new kgmLuaScript(g->getResources());

  handler->load("main");
}

kgmGameScript::~kgmGameScript()
{
  delete handler;
}

void kgmGameScript::update()
{
  handler->call("main_update", "");
}
