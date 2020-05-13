#include "kgmGameScript.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "kgmGameTools.h"
#include "kgmGameRetention.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmScript/kgmLuaScript.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiMenu.h"
#include "../kgmGraphics/kgmGuiText.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiCheck.h"
#include "../kgmGraphics/kgmGuiLayout.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGuiProgress.h"
#include "../kgmSystem/kgmWindow.h"

s8* kgmGameScript::value = null;

kgmGameScript::kgmGameScript(kgmIGame* g)
{
  game = g;

  status = false;

  handler = new kgmLuaScript(g->getResources());

  handler->setX(this);

  mutex = kgmThread::mutex_create();
}

kgmGameScript::~kgmGameScript()
{
  kgmThread::mutex_lock(mutex);

  kgmThread::mutex_free(mutex);

  free();

  for(kgmList< Slot<kgmGameScript>* >::iterator i = slots.begin(); !i.end(); ++i)
    delete (*i);

  delete handler;
}

void kgmGameScript::init()
{

  handler->set("kgmLog",   kgmGameScript::kgmLog);

  handler->set("kgmImport",   kgmGameScript::kgmImport);

  handler->set("kgmGameExit",   kgmGameScript::kgmGameExit);
  handler->set("kgmGameLoad",   kgmGameScript::kgmGameLoad);
  handler->set("kgmGamePlay",   kgmGameScript::kgmGamePlay);
  handler->set("kgmGamePause",  kgmGameScript::kgmGamePause);
  handler->set("kgmGameState",  kgmGameScript::kgmGameState);
  handler->set("kgmGameUnits",  kgmGameScript::kgmGameUnits);

  handler->set("kgmGuiLoad",  kgmGameScript::kgmGuiLoad);
  handler->set("kgmGuiShow",  kgmGameScript::kgmGuiShow);
  handler->set("kgmGuiResize",  kgmGameScript::kgmGuiResize);
  handler->set("kgmGuiResolution",  kgmGameScript::kgmGuiResolution);
  handler->set("kgmGuiSetVAlign",  kgmGameScript::kgmGuiSetVAlign);
  handler->set("kgmGuiSetHAlign",  kgmGameScript::kgmGuiSetHAlign);
  handler->set("kgmGuiGetChild",  kgmGameScript::kgmGuiGetChild);
  handler->set("kgmGuiCallback",  kgmGameScript::kgmGuiCallback);

  handler->set("kgmGenRetention",  kgmGameScript::kgmGenRetention);
  handler->set("kgmDelRetention",  kgmGameScript::kgmDelRetention);
  handler->set("kgmGetRetention",  kgmGameScript::kgmGetRetention);
  handler->set("kgmSetRetention",  kgmGameScript::kgmSetRetention);

  handler->set("kgmScreenResolution",  kgmGameScript::kgmScreenResolution);

  status = handler->load("main");

  if (status)
    handler->call("main_init", "");
}

void kgmGameScript::free()
{
  if (status)
    handler->call("main_free", "");
}

void kgmGameScript::update()
{
  kgmThread::mutex_lock(mutex);

  if (status)
    handler->call("main_update", "i", game->timeUpdate());

  kgmThread::mutex_unlock(mutex);
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
    slots.add( (Slot<kgmGameScript>*) slotGuiButton);
  }
}

void kgmGameScript::lock()
{
  kgmThread::mutex_lock(mutex);
}

void kgmGameScript::unlock()
{
  kgmThread::mutex_unlock(mutex);
}

void kgmGameScript::onButton(s32 key, s32 btn, s32 down)
{
  if (status && handler)
    handler->call("main_button", "iii", key, btn, down);
}

__stdcall void kgmGameScript::onQuit()
{
}

__stdcall void kgmGameScript::onLoad()
{
  lock();
  if (status && handler)
    handler->call("main_load", "");
  unlock();
}

__stdcall void kgmGameScript::onUnload()
{
  lock();
  if (status && handler)
    handler->call("main_unload", "");
  unlock();
}

__stdcall void kgmGameScript::onInsert(kgmUnit* u)
{
  lock();
  if (status && handler)
    handler->call("main_insert", "p", u);
  unlock();
}

__stdcall void kgmGameScript::onRemove(kgmUnit* u)
{
  lock();
  if (status && handler)
    handler->call("main_remove", "p", u);
  unlock();
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

s32 kgmGameScript::kgmLog(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s8* log = null;

  game->getScript()->args("s", &log);

  kgm_log_print(log);

  return 0;
}

s32 kgmGameScript::kgmImport(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s8* script = null;

  game->getScript()->args("s", &script);

  if (script)
  {
    game->getScript()->load(script);
  }

  return 0;
}

s32 kgmGameScript::kgmGameExit(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  if(game->gState() != kgmIGame::State_Quit)
    game->gQuit();

  return 0;
}

s32 kgmGameScript::kgmGamePlay(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->gSwitch(kgmIGame::State_Play);

  return 0;
}

s32 kgmGameScript::kgmGamePause(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->gSwitch(kgmIGame::State_Pause);

  return 0;
}

s32 kgmGameScript::kgmGameState(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  state = game->gState();

  game->getScript()->resl("i", state);

  return 1;
}

s32 kgmGameScript::kgmGameLoad(void *)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s8* map = null;

  game->getScript()->args("s", &map);

  if (map)
    state = game->gLoad(map);

  game->getScript()->resl("i", state);

  return 1;
}

s32 kgmGameScript::kgmGameUnits(void *)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s32 i = 0;

  kgmArray<kgmUnit*> units;
  kgmIGame::Iterator it = game->gUnits();

  units.realloc(128);

  while(kgmUnit* u = it.next())
  {
    if (i == units.length())
      units.realloc(units.length() + 128);

    units[i] = u;
  }

  units.realloc(i + 1);

  game->getScript()->reslarr("p", units.data(), units.length());

  return 1;
}

s32 kgmGameScript::kgmGenRetention(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmGameRetention* ret = null;

  s8* name = null;

  game->getScript()->args("s", &name);

  if (name)
  {
    ret = new kgmGameRetention();
    ret->name(name);
    ret->encryptor(83, 107, 643, 7935);
    ret->load();
  }

  game->getScript()->resl("p", ret);

  return 1;
}

s32 kgmGameScript::kgmDelRetention(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmGameRetention* ret = null;

  game->getScript()->args("p", &ret);

  if (ret)
  {
    ret->keep();

    delete ret;
  }

  return 0;
}

s32 kgmGameScript::kgmGetRetention(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmGameRetention* ret = null;

  s8* key = null;

  game->getScript()->args("ps", &ret, &key);

  if (ret && key)
  {
    value = ret->get(key);
  }

  game->getScript()->resl("s", value);

  return 1;
}

s32 kgmGameScript::kgmSetRetention(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmGameRetention* ret = null;

  s8* val = null;;

  s8* key = null;

  game->getScript()->args("pss", &ret, &key, &val);

  if (ret && key && val)
  {
    ret->set(key, val);
  }

  return 0;
}

int kgmGameScript::kgmGuiLoad(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

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

  return 1;
}

s32 kgmGameScript::kgmGuiShow(void*)
{
  kgmGui* gui  = null;
  u32     show = -1;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->getScript()->args("pi", &gui, &show);

  if (gui && show != -1)
  {
    if (show)
      gui->show();
    else
      gui->hide();
  }

  return 0;
}

s32 kgmGameScript::kgmGuiResize(void*)
{
  kgmGui* gui  = null;
  double  sw = 1.0, sh = 1.0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->getScript()->args("pdd", &gui, &sw, &sh);

  if (gui)
  {
    kgmGui::Rect rc;

    gui->getRect(rc);

    rc.w = sw;
    rc.h = sh;

    gui->setRect(rc);
  }
  else
  {
    kgm_log() << "No gui for scale. \n";
  }

  return 0;
}

s32 kgmGameScript::kgmGuiResolution(void*)
{
  kgmGui* gui  = null;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->getScript()->args("p", &gui);

  if (gui)
  {
    kgmGui::Rect rc = gui->getRect();

    game->getScript()->resl("ii", rc.width(), rc.height());
  }

  return 2;
}

s32 kgmGameScript::kgmGuiSetVAlign(void*)
{
  kgmGui* gui  = null;
  s32  a;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->getScript()->args("pi", &gui, &a);

  if (gui)
  {
    ((kgmGuiLayout*)gui)->setVAlign(a);
  }

  return 0;
}

s32 kgmGameScript::kgmGuiSetHAlign(void*)
{
  kgmGui* gui  = null;
  s32  a;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->getScript()->args("pi", &gui, &a);

  if (gui)
  {
    ((kgmGuiLayout*)gui)->setHAlign(a);
  }

  return 0;
}

int kgmGameScript::kgmGuiGetChild(void*)
{
  kgmGui* gui = null;
  s8* id = null;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
  {
    game->getScript()->resl("p", null);

    return 1;
  }

  game->getScript()->args("ps", &gui, &id);

  if (!gui || !id)
  {
    game->getScript()->resl("p", null);

    return 1;
  }

  kgmGui* child = ((kgmGuiLayout*)gui)->getChild(id);

  game->getScript()->resl("p", child);

  return 1;
}

s32 kgmGameScript::kgmGuiCallback(void*)
{
  kgmGui* gui = null;
  s8* fn = null;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
  {
    return 0;
  }

  game->getScript()->args("ps", &gui, &fn);

  if (gui && fn)
   ((kgmGameScript*) game->getScript())->setSlot(gui, fn);

  return 0;
}

s32 kgmGameScript::kgmScreenResolution(void*)
{
  kgmGui* gui  = null;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  int rc[4];

  game->getWindow()->getRect(rc[0], rc[1], rc[2], rc[3]);

  game->getScript()->resl("ii", rc[2], rc[3]);

  return 2;
}

