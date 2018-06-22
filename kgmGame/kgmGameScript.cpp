#include "kgmGameScript.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "kgmGameTools.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmScript/kgmLuaScript.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiMenu.h"
#include "../kgmGraphics/kgmGuiText.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiCheck.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGuiProgress.h"

kgmGameScript::kgmGameScript(kgmIGame* g)
{
  game = g;

  handler = new kgmLuaScript(g->getResources());

  handler->setX(this);
}

kgmGameScript::~kgmGameScript()
{
  free();

  delete handler;
}

void kgmGameScript::init()
{

  handler->set("kgmLog",   kgmGameScript::kgmLog);

  handler->set("kgmImport",   kgmGameScript::kgmImport);

  handler->set("kgmGameExit",   kgmGameScript::kgmGameExit);
  handler->set("kgmGamePlay",   kgmGameScript::kgmGamePlay);
  handler->set("kgmGamePause",  kgmGameScript::kgmGamePause);
  handler->set("kgmGameState",  kgmGameScript::kgmGameState);

  handler->set("kgmGuiLoad",  kgmGameScript::kgmGuiLoad);
  handler->set("kgmGuiShow",  kgmGameScript::kgmGuiShow);

  handler->load("main");

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

void kgmGameScript::setSlot(kgmGui* gui, kgmString call)
{
  if (!gui || call.length() < 1)
    return;

  if (!strcmp(gui->vClass(), "kgmGuiButton"))
  {
    Slot<kgmGameScript, int>* slotGuiButton = new Slot<kgmGameScript, int>();
    slotGuiButton->connect(this, (Slot<kgmGameScript, int>::FNS) &kgmGameScript::onSlotGuiButton,
                           gui, &((kgmGuiButton*)gui)->sigClick);

    slotters.set(gui, call);
  }
}

__stdcall void kgmGameScript::onSlotGuiList(kgmGui* g, u32 s)
{
  if (!g)
    return;

  kgmMap<kgmGui*, kgmString>::iterator i = slotters.get(g);

  if (!i.isValid())
    return;

  handler->call(i.data(), "i", s);
}

__stdcall void kgmGameScript::onSlotGuiMenu(kgmGui* g, u32 s)
{
  if (!g)
    return;

  kgmMap<kgmGui*, kgmString>::iterator i = slotters.get(g);

  if (!i.isValid())
    return;

  handler->call(i.data(), "i", s);
}

__stdcall void kgmGameScript::onSlotGuiText(kgmGui* g, kgmString s)
{
  if (!g)
    return;

  kgmMap<kgmGui*, kgmString>::iterator i = slotters.get(g);

  if (!i.isValid())
    return;

  handler->call(i.data(), "s", s.data());
}

__stdcall void kgmGameScript::onSlotGuiCheck(kgmGui* g, bool n)
{
  if (!g)
    return;

  kgmMap<kgmGui*, kgmString>::iterator i = slotters.get(g);

  if (!i.isValid())
    return;

  handler->call(i.data(), "i", n);
}

__stdcall void kgmGameScript::onSlotGuiButton(kgmGui* s, int n)
{
  if (!s)
    return;

  kgmMap<kgmGui*, kgmString>::iterator i = slotters.get(s);

  if (!i.isValid())
    return;

  handler->call(i.data(), "i", n);
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

void kgmGameScript::kgmImport(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  s8* script = null;

  game->getScript()->args("s", &script);

  if (script)
  {
    game->getScript()->load(script);
  }
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

void kgmGameScript::kgmGuiLoad(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  s8* sid = null;

  kgmGui* gui = null;

  game->getScript()->args("s", &sid);

  if (sid)
  {
    kgmMemory<u8> mem;
    game->getResources()->getFile(sid, mem);

    kgmXml xml;

    xml.open(mem);

    gui = kgmGameTools::genGui((kgmGameScript *)game->getScript()->getX(), xml);

    if (gui)
      game->guiAdd(gui);
  }

  game->getScript()->resl("p", gui);
}

void kgmGameScript::kgmGuiShow(void*)
{
  kgmGui* gui  = null;
  u32     show = -1;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return;

  game->getScript()->args("pi", &gui, &show);

  if (gui && show != -1)
  {
    if (show)
      gui->show();
    else
      gui->hide();
  }
}
