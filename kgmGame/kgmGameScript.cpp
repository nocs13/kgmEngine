#include "kgmGameScript.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "kgmGameTools.h"
#include "kgmGameRetention.h"
#include "kgmUnit.h"
#include "kgmGameInput.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmScript/kgmLuaScript.h"
#include "../kgmGraphics/kgmIGraphics.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiMenu.h"
#include "../kgmGraphics/kgmGuiText.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiCheck.h"
#include "../kgmGraphics/kgmGuiLayout.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGuiProgress.h"
#include "../kgmGraphics/kgmGuiContainer.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmSystem/kgmWindow.h"
#include "../kgmSystem/kgmProcess.h"

//#define DEBUG2
#undef DEBUG2

char* kgmGameScript::value = null;

kgmGameScript::kgmGameScript(kgmIGame* g)
{
  game = g;

  status = false;

  handler = new kgmLuaScript(g->getResources());

  handler->setX(this);

  mutex = kgmThread::mutex_create(true);
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
  handler->set("kgmGameUnload", kgmGameScript::kgmGameUnload);
  handler->set("kgmGameCamera", kgmGameScript::kgmGameCamera);

  handler->set("kgmCamPosition", kgmGameScript::kgmCamPosition);
  handler->set("kgmCamDirection", kgmGameScript::kgmCamDirection);
  handler->set("kgmCamMove", kgmGameScript::kgmCamMove);
  handler->set("kgmCamLook", kgmGameScript::kgmCamLook);

  handler->set("kgmUnitId", kgmGameScript::kgmUnitId);
  handler->set("kgmUnitName", kgmGameScript::kgmUnitName);
  handler->set("kgmUnitClass", kgmGameScript::kgmUnitClass);

  handler->set("kgmGuiLoad",  kgmGameScript::kgmGuiLoad);
  handler->set("kgmGuiShow",  kgmGameScript::kgmGuiShow);
  handler->set("kgmGuiResize",  kgmGameScript::kgmGuiResize);
  handler->set("kgmGuiResolution",  kgmGameScript::kgmGuiResolution);
  handler->set("kgmGuiSetVAlign",  kgmGameScript::kgmGuiSetVAlign);
  handler->set("kgmGuiSetHAlign",  kgmGameScript::kgmGuiSetHAlign);
  handler->set("kgmGuiGetChild",  kgmGameScript::kgmGuiGetChild);
  handler->set("kgmGuiCallback",  kgmGameScript::kgmGuiCallback);
  handler->set("kgmGuiCreate",  kgmGameScript::kgmGuiCreate);
  handler->set("kgmGuiAdd",  kgmGameScript::kgmGuiAdd);
  handler->set("kgmGuiSetText",  kgmGameScript::kgmGuiSetText);
  handler->set("kgmGuiSetWindow",  kgmGameScript::kgmGuiSetWindow);
//  handler->set("kgmGuiSetCallback",  kgmGameScript::kgmGuiSetCallback);

  handler->set("kgmGenRetention",  kgmGameScript::kgmGenRetention);
  handler->set("kgmDelRetention",  kgmGameScript::kgmDelRetention);
  handler->set("kgmGetRetention",  kgmGameScript::kgmGetRetention);
  handler->set("kgmSetRetention",  kgmGameScript::kgmSetRetention);

  handler->set("kgmScreenResolution",  kgmGameScript::kgmScreenResolution);

  handler->set("kgmKeyState",  kgmGameScript::kgmKeyState);
  handler->set("kgmPointState",  kgmGameScript::kgmPointState);

  handler->set("kgmRunProcess",  kgmGameScript::kgmRunProcess);

  slotters.clear();

  status = handler->load("main");

  if (status)
    handler->call("main_oninit", "");
}

void kgmGameScript::free()
{
  if (status)
    handler->call("main_onfree", "");
}

void kgmGameScript::update()
{
#ifdef DEBUG2
  kgm_log() << "Call main_onupdate. \n";
#endif

  lock();
  locks[1] ++;
  if (status)
    handler->call("main_onupdate", "i", game->timeUpdate());

  unlock();
  locks[1] --;
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
  kgmThread::TID thread = kgmThread::id();

  if (mlocker == thread)
    return;

#ifdef DEBUG2
  kgm_log() << "kgmGameScript::locking...\n";
#endif

  kgmThread::mutex_lock(mutex);

#ifdef DEBUG2
  kgm_log() << "kgmGameScript::locked.\n";
#endif

  mlocker = kgmThread::id();
}

void kgmGameScript::unlock()
{
  kgmThread::TID thread = kgmThread::id();

  if (mlocker != thread)
    return;


#ifdef DEBUG2
  kgm_log() << "kgmGameScript::unlocked.\n";
#endif

  mlocker = -1;
  kgmThread::mutex_unlock(mutex);
}

bool kgmGameScript::locked()
{
  if (kgmThread::mutex_locked(mutex))
  {
    kgm_log() << "Game script Thread is locked.\n";

    return true;
  }

  kgm_log() << "Game script Thread not locked.\n";

  return false;
}

void kgmGameScript::onButton(s32 key, s32 btn, s32 down)
{
  kgmThread::TID ctd = kgmThread::id();

  if (ctd == mlocker)
    return;

  lock();
  locks[0] ++;


  if (status && handler)
    handler->call("main_onbutton", "iii", key, btn, down);

  unlock();
  locks[0] --;
}

__stdcall void kgmGameScript::onQuit()
{
}

__stdcall void kgmGameScript::onLoad()
{
  if (status && handler)
    handler->call("main_onload", "");
}

__stdcall void kgmGameScript::onUnload()
{
  if (status && handler)
    handler->call("main_onunload", "");
}

__stdcall void kgmGameScript::onInsert(kgmUnit* u)
{
  if (status && handler)
    handler->call("main_oninsert", "p", u);
}

__stdcall void kgmGameScript::onRemove(kgmUnit* u)
{
  if (status && handler)
    handler->call("main_onremove", "p", u);
}

__stdcall void kgmGameScript::onSlotGuiList(kgmGui* g, u32 s)
{
  if (!g)
    return;

  auto i = slotters.get(g);

  if (!i.isValid())
    return;

  handler->call(i.data(), "i", s);
}

__stdcall void kgmGameScript::onSlotGuiMenu(kgmGui* g, u32 s)
{
  if (!g)
    return;

  auto i = slotters.get(g);

  if (!i.isValid())
    return;

  handler->call(i.data(), "i", s);
}

__stdcall void kgmGameScript::onSlotGuiText(kgmGui* g, kgmString s)
{
  if (!g)
    return;

  auto i = slotters.get(g);

  if (!i.isValid())
    return;

  handler->call(i.data(), "s", s.data());
}

__stdcall void kgmGameScript::onSlotGuiCheck(kgmGui* g, bool n)
{
  if (!g)
    return;

  auto i = slotters.get(g);

  if (!i.isValid())
    return;

  handler->call(i.data(), "i", n);
}

__stdcall void kgmGameScript::onSlotGuiButton(kgmGui* s, int n)
{
  if (!s)
    return;

  auto i = slotters.get(s);

  if (!i.isValid())
    return;

  lock();
  locks[0] ++;

  handler->call(i.data(), "i", n);

  unlock();
  locks[0] --;
}

s32 kgmGameScript::kgmLog(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s8* log = null;

  game->getScript()->args("s", &log);

  kgm_log() << log << "\n";

  return 0;
}

s32 kgmGameScript::kgmImport(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  char* script = null;

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

  game->gPause(false);

  state = game->gState();

  if (state == kgmIGame::State_Play)
    state = 1;
  else
    state = 0;

  game->getScript()->resl("i", state);

  return 1;
}

s32 kgmGameScript::kgmGamePause(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->gPause(true);

  state = game->gState();

  if (state == kgmIGame::State_Pause)
    state = 1;
  else
    state = 0;

  game->getScript()->resl("i", state);

  return 1;
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

  char* map = null;

  game->getScript()->args("s", &map);

  if (map)
    state = game->gLoad(map);

  game->getScript()->resl("i", state);

  return 1;
}

s32 kgmGameScript::kgmGameUnload(void *)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  state = game->gUnload();

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
  kgmIGame::Iterator* it = game->gUnits();

  units.realloc(128);

  while(kgmUnit* u = it->next())
  {
    if (i == units.length())
      units.realloc(units.length() + 128);

    units[i] = u;
  }

  units.realloc(i + 1);

  game->getScript()->reslarr("p", units.data(), units.length());

  return 1;
}

s32 kgmGameScript::kgmGameCamera(void*)
{
  kgmCamera* cam = null;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  cam = &game->getGraphics()->camera();

  game->getScript()->resl("p", cam);

  return 1;
}

s32 kgmGameScript::kgmCamMove(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmCamera* cam    = null;
  f64        pos[3] = {0.0};

  game->getScript()->args("pddd", &cam, &pos[0], &pos[1], &pos[2]);

  if (!cam)
    cam = &game->getGraphics()->camera();

  cam->move(pos[0], pos[1], pos[2]);

  return 0;
}

s32 kgmGameScript::kgmCamLook(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmCamera* cam    = null;
  f64        dir[3] = {0.0};

  game->getScript()->args("pddd", &cam, &dir[0], &dir[1], &dir[2]);

  if (!cam)
    cam = &game->getGraphics()->camera();

  cam->look(dir[0], dir[1], dir[2]);

  return 0;
}

s32 kgmGameScript::kgmCamPosition(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmCamera* cam    = null;

  game->getScript()->args("p", &cam);

  if (!cam)
    cam = &game->getGraphics()->camera();

  vec3 v = cam->mPos;

  game->getScript()->resl("ddd", v.x, v.y, v.z);

  return 3;
}

s32 kgmGameScript::kgmCamDirection(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmCamera* cam    = null;

  game->getScript()->args("p", &cam);

  if (!cam)
    cam = &game->getGraphics()->camera();

  vec3 v = cam->mDir;

  game->getScript()->resl("ddd", v.x, v.y, v.z);

  return 3;
}

s32 kgmGameScript::kgmUnitId(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("u", &u);

  if (!u)
    return 0;

  game->getScript()->resl("i", u->getId());

  return 1;
}

s32 kgmGameScript::kgmUnitName(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("p", &u);

  if (!u)
    return 0;

  game->getScript()->resl("s", u->getName().data());

  return 1;
}

s32 kgmGameScript::kgmUnitClass(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("p", &u);

  if (!u)
    return 0;

  game->getScript()->resl("s", u->getClass().data());

  return 1;
}

s32 kgmGameScript::kgmGenRetention(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmGameRetention* ret = null;

  char* name = null;

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

  char* key = null;

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

  char* val = null;

  char* key = null;

  game->getScript()->args("pss", &ret, &key, &val);

  if (ret && key && val)
  {
    ret->set(key, val);
  }

  return 0;
}

s32 kgmGameScript::kgmGuiLoad(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  char* sid = null;

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
  s32     show = -1;

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
  char* id = null;

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

  kgmGui* child = gui->getBySid(id);

  game->getScript()->resl("p", child);

  return 1;
}

s32 kgmGameScript::kgmGuiCallback(void*)
{
  kgmGui* gui = null;
  char* fn = null;

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

s32 kgmGameScript::kgmGuiCreate(void*)
{
  kgmGui* gui = null;
  s32 w, h;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
  {
    return 0;
  }

  game->getScript()->args("ii", &w, &h);

  gui = new kgmGuiContainer(null, 0, 0, w, h);

  if (gui)
    game->guiAdd(gui);

  game->getScript()->resl("p", gui);

  return 1;
}

s32 kgmGameScript::kgmGuiAdd(void*)
{
  kgmGui* child = null;
  s32 w, h;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
  {
    return 0;
  }

  kgmGuiContainer* g = null;
  char* t = null;
  char* id = null;
  game->getScript()->args("pssi", &g, &t, &id, &w);

  if (!g)
    return 0;

  kgmString type(t);

  if (type == "Label") {
    child = new kgmGuiLabel(null, 0, 0, 0, 0);
    g->add(child, (kgmGuiContainer::CellSize) w, (kgmGuiContainer::Align) 0);
  } else if (type == "Check") {
    child = new kgmGuiCheck(null, 0, 0, 0, 0);
    g->add(child, (kgmGuiContainer::CellSize) w, (kgmGuiContainer::Align) 0);
  } else if (type == "Button") {
    child = new kgmGuiButton(null, 0, 0, 0, 0);
    g->add(child, (kgmGuiContainer::CellSize) w, (kgmGuiContainer::Align) 0);
  } else if (type == "Progress") {
    child = new kgmGuiProgress(null, 0, 0, 0, 0);
    g->add(child, (kgmGuiContainer::CellSize) w, (kgmGuiContainer::Align) 0);
  } else {
    g->add(null, (kgmGuiContainer::CellSize) w, (kgmGuiContainer::Align) 0);
  }

  s32 res = 0;

  if (child) {
    child->setSid(id);
    game->getScript()->resl("p", child);
    res = 1;
  }

  return res;
}

s32 kgmGameScript::kgmGuiSetText(void*)
{
  kgmGui* child = null;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
  {
    return 0;
  }

  kgmGuiContainer* g = null;
  char* id = null;
  char* txt = null;
  game->getScript()->args("pss", &g, &id, &txt);

  if (!g)
    return 0;

  child = g->getBySid(id);

  if (child)
    child->setText(txt);

  return 0;
}

s32 kgmGameScript::kgmGuiSetWindow(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
  {
    return 0;
  }

  kgmGuiContainer* g = null;
  game->getScript()->args("p", &g);

  if (!g)
    return 0;

  g->setWindow(game->getWindow());

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

s32 kgmGameScript::kgmKeyState(void*)
{
  s32 key = -1;
  s32 state = -1;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->getScript()->args("i", &key);

  kgmIInput* input = game->getInput();

  if (!input)
    return 0;

  state = input->keyState(key);

  game->getScript()->resl("i", state);

  return 1;
}

s32 kgmGameScript::kgmPointState(void*)
{
  s32 dev = -1;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->getScript()->args("i", &dev);

  kgmIInput* input = game->getInput();

  if (!input)
    return 0;

  kgmIInput::Point pt = input->pointState(dev);

  game->getScript()->resl("iii", pt.x, pt.y, pt.z);

  return 3;
}

s32 kgmGameScript::kgmRunProcess(void*)
{
  kgmProcess proc;

   kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  char* cmd;
  char* arg;

  game->getScript()->args("ss", &cmd, &arg);

  bool res = proc.run(cmd, arg);

  if (res)
  {
    static kgmString s = proc.out();

    game->getScript()->resl("is", 1, s.data());
  } 
  else
  {
    game->getScript()->resl("i", -1);
  }

  return 1;
}
