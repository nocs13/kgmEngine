#include "kgmGameScript.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "kgmGameTools.h"
#include "kgmGameGraphics.h"
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

kgmString option;

kgmGameScript::kgmGameScript(kgmIGame* g)
{
  game = g;

  status = false;

  handler = new kgmLuaScript(g->getResources());

  handler->setX(this);
}

kgmGameScript::~kgmGameScript()
{
  clear();

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
  handler->set("kgmGameUnload", kgmGameScript::kgmGameUnload);
  handler->set("kgmGameCamera", kgmGameScript::kgmGameCamera);
  handler->set("kgmGameSetState",  kgmGameScript::kgmGameSetState);

  handler->set("kgmCamPosition", kgmGameScript::kgmCamPosition);
  handler->set("kgmCamDirection", kgmGameScript::kgmCamDirection);
  handler->set("kgmCamMove", kgmGameScript::kgmCamMove);
  handler->set("kgmCamLook", kgmGameScript::kgmCamLook);

  handler->set("kgmMtlGet", kgmGameScript::kgmMtlGet);
  handler->set("kgmMtlSetType", kgmGameScript::kgmMtlSetType);
  handler->set("kgmMtlSetCull", kgmGameScript::kgmMtlSetCull);
  handler->set("kgmMtlSetMaps", kgmGameScript::kgmMtlSetMaps);
  handler->set("kgmMtlSetAlpha", kgmGameScript::kgmMtlSetAlpha);
  handler->set("kgmMtlSetColor", kgmGameScript::kgmMtlSetColor);

  handler->set("kgmLightGet", kgmGameScript::kgmLightGet);
  handler->set("kgmLightSetForce", kgmGameScript::kgmLightSetForce);
  handler->set("kgmLightSetColor", kgmGameScript::kgmLightSetColor);

  handler->set("kgmMeshGet", kgmGameScript::kgmMeshGet);

  handler->set("kgmUnitId", kgmGameScript::kgmUnitId);
  handler->set("kgmUnitName", kgmGameScript::kgmUnitName);
  handler->set("kgmUnitType", kgmGameScript::kgmUnitType);
  handler->set("kgmUnitGroup", kgmGameScript::kgmUnitGroup);
  handler->set("kgmUnitCreate", kgmGameScript::kgmUnitCreate);
  handler->set("kgmUnitRemove", kgmGameScript::kgmUnitRemove);
  handler->set("kgmUnitSetMesh", kgmGameScript::kgmUnitSetMesh);
  handler->set("kgmUnitSetLight", kgmGameScript::kgmUnitSetLight);
  handler->set("kgmUnitGetValue", kgmGameScript::kgmUnitGetValue);
  handler->set("kgmUnitSetValue", kgmGameScript::kgmUnitSetValue);
  handler->set("kgmUnitSetScale", kgmGameScript::kgmUnitSetScale);
  handler->set("kgmUnitGetScale", kgmGameScript::kgmUnitGetScale);
  handler->set("kgmUnitGetPosition", kgmGameScript::kgmUnitGetPosition);
  handler->set("kgmUnitSetPosition", kgmGameScript::kgmUnitSetPosition);
  handler->set("kgmUnitGetRotation", kgmGameScript::kgmUnitGetRotation);
  handler->set("kgmUnitSetRotation", kgmGameScript::kgmUnitSetRotation);

  handler->set("kgmUnitIterator", kgmGameScript::kgmUnitIterator);
  handler->set("kgmUnitIterNext", kgmGameScript::kgmUnitIterNext);
  handler->set("kgmUnitIterFree", kgmGameScript::kgmUnitIterFree);

  handler->set("kgmGuiLoad",  kgmGameScript::kgmGuiLoad);
  handler->set("kgmGuiShow",  kgmGameScript::kgmGuiShow);
  handler->set("kgmGuiResize",  kgmGameScript::kgmGuiResize);
  handler->set("kgmGuiResolution",  kgmGameScript::kgmGuiResolution);
  handler->set("kgmGuiGetChild",  kgmGameScript::kgmGuiGetChild);
  handler->set("kgmGuiCallback",  kgmGameScript::kgmGuiCallback);
  handler->set("kgmGuiCreate",  kgmGameScript::kgmGuiCreate);
  handler->set("kgmGuiAdd",  kgmGameScript::kgmGuiAdd);
  handler->set("kgmGuiSetText",  kgmGameScript::kgmGuiSetText);
  handler->set("kgmGuiSetWindow",  kgmGameScript::kgmGuiSetWindow);

  handler->set("kgmGenRetention",  kgmGameScript::kgmGenRetention);
  handler->set("kgmDelRetention",  kgmGameScript::kgmDelRetention);
  handler->set("kgmGetRetention",  kgmGameScript::kgmGetRetention);
  handler->set("kgmSetRetention",  kgmGameScript::kgmSetRetention);

  handler->set("kgmGcSetBgColor",  kgmGameScript::kgmGcSetBgColor);

  handler->set("kgmScreenResolution",  kgmGameScript::kgmScreenResolution);

  handler->set("kgmKeyState",  kgmGameScript::kgmKeyState);
  handler->set("kgmPointState",  kgmGameScript::kgmPointState);

  handler->set("kgmRunProcess",  kgmGameScript::kgmRunProcess);

  status = handler->load("main");

  if (status)
    handler->call("main_oninit", "");
}

void kgmGameScript::clear()
{
  if (status)
    handler->call("main_onfree", "");
}

void kgmGameScript::update()
{
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

__stdcall void kgmGameScript::onQuit()
{
  if (status && handler)
    handler->call("main_onquit", "");
}

__stdcall void kgmGameScript::onLoad()
{
  if (status && handler)
    handler->call("main_onload", "");
}

__stdcall void kgmGameScript::onPlay()
{
  if (handler)
  {
    handler->call("main_onplay", "");
  }
}

__stdcall void kgmGameScript::onPause()
{
  if (status && handler)
    handler->call("main_onpause", "");
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

  handler->call(i.data(), "i", n);
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

s32 kgmGameScript::kgmGameSetState(void*)
{
  s32 state = 0;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  game->getScript()->args("i", &state);

  game->gSwitch(state);

  game->getScript()->resl("i", 1);

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

s32 kgmGameScript::kgmUnitIterator(void *)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s32 i = 0;

  kgmIGame::Iterator* it = game->gUnits();

  game->getScript()->resl("p", it);

  return 1;
}

s32 kgmGameScript::kgmUnitIterNext(void *)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s32 i = 0;

  kgmIGame::Iterator* it = null;

  game->getScript()->args("p", &it);

  if (!it) {
    return 0;
  }

  kgmUnit* u = it->next();

  if (!u) {
    return 0;
  }

  game->getScript()->resl("p", u);

  return 1;
}

s32 kgmGameScript::kgmUnitIterFree(void *)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s32 i = 0;

  kgmIGame::Iterator* it = null;

  game->getScript()->args("p", &it);

  if (!it) {
    return 0;
  }

  delete it;

  return 0;
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

s32 kgmGameScript::kgmMtlGet(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  const char *id = null;

  game->getScript()->args("s", &id);

  kgmMaterial* m = new kgmMaterial();

  m->setId((id == null) ? ("") : (id));

  game->getScript()->resl("p", m);

  return 1;
}

s32 kgmGameScript::kgmMtlSetMaps(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmMaterial* m = null;
  const char *c = null, *s = null, *n = null;

  game->getScript()->args("psss", &m, &c, &n, &s);

  if (!m)
    return 0;

  if (c != null && strlen(c) > 0)
    m->setTexColor(game->getResources()->getTexture(c));
  if (n != null && strlen(n) > 0)
    m->setTexNormal(game->getResources()->getTexture(n));
  if (s != null && strlen(s) > 0)
    m->setTexSpecular(game->getResources()->getTexture(s));

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmMtlSetCull(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmMaterial* m = null;
  s32 c;

  game->getScript()->args("pi", &m, &c);

  if (!m)
    return 0;

  m->cull((c == 0) ? (false) : (true));

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmMtlSetColor(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmMaterial* m = null;
  double r, g, b;

  game->getScript()->args("pddd", &m, &r, &g, &b);

  if (!m)
    return 0;

  m->color(r, g, b, 1.0);

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmMtlSetAlpha(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmMaterial* m = null;
  double a;

  game->getScript()->args("pd", &m, &a);

  if (!m)
    return 0;

  m->transparency(a);

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmMtlSetType(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmMaterial* m = null;
  s32 t;

  game->getScript()->args("pi", &m, &t);

  if (!m || !t)
    return 0;

  kgmMaterial::Type type = static_cast<kgmMaterial::Type>(t);

  m->type(type);

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmLightGet(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  s32 tp = null;

  game->getScript()->args("i", &tp);

  kgmLight* lt = new kgmLight();

  lt->type(static_cast<kgmLight::Type>(tp));

  game->getScript()->resl("p", lt);

  return 1;
}

s32 kgmGameScript::kgmLightSetForce(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmLight* lt = null;
  double f;

  game->getScript()->args("pd", &lt, &f);

  if (!lt)
    return 0;

  lt->intensity(f);

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmLightSetColor(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmLight* lt = null;
  double r, g, b;

  game->getScript()->args("pddd", &lt, &r, &g, &b);

  if (!lt)
    return 0;

  vec3 v(r, g, b);
  lt->color(v);

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmMeshGet(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  const char *id = null;

  game->getScript()->args("s", &id);

  if (!id)
    return 0;

  kgmMesh* m = game->getResources()->getMesh(id);

  if (!m)
    return 0;

  game->getScript()->resl("p", m);

  return 1;
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

s32 kgmGameScript::kgmUnitType(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("p", &u);

  if (!u)
    return 0;

  game->getScript()->resl("s", u->getType().data());

  return 1;
}

s32 kgmGameScript::kgmUnitGroup(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("p", &u);

  if (!u)
    return 0;

  game->getScript()->resl("s", u->getGroup().data());

  return 1;
}

s32 kgmGameScript::kgmUnitCreate(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  const char *name = null, *type = null;
  game->getScript()->args("ss", &name, &type);

  kgmUnit* u = new kgmUnit(game);

  u->setType(type);
  u->setName(name);

  if (game->gAppend(u) != true) {
    u->release();

    return 0;
  }

  game->getScript()->resl("p", u);

  return 1;
}

s32 kgmGameScript::kgmUnitRemove(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("p", &u);

  if (!u)
    return 0;

  u->remove();

  return 0;
}

s32 kgmGameScript::kgmUnitGetValue(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  const char* id = null;

  game->getScript()->args("ps", &u, &id);

  if (!u || !id)
    return 0;

  option = u->getOption(id);

  game->getScript()->resl("s", option.data());

  return 1;
}

s32 kgmGameScript::kgmUnitSetValue(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  const char* id = null;
  const char* val = null;

  game->getScript()->args("pss", &u, &id, &val);

  if (!u || !id || !val)
    return 0;

  u->setOption(id, val);

  return 0;
}

s32 kgmGameScript::kgmUnitSetBody(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  const char* id = null;

  game->getScript()->args("ps", &u, &id);

  if (!u || !id)
    return 0;

  if (u->getBody()) {
    kgmPBody* b = static_cast<kgmPBody*>(u->getBody());

    b->release();
  }

  //kgmPBody* b = new kgmPBody(u);

  //u->setBody(b);

  return 0;
}

s32 kgmGameScript::kgmUnitSetLight(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  kgmLight* l = null;

  game->getScript()->args("pp", &u, &l);

  if (!u || !l)
    return 0;

  kgmGNode* tn = static_cast<kgmGNode*>(u->getNode());
  kgmGNode* n = new kgmGNode(u, l, kgmIGraphics::NodeLight);
  u->setNode(n);

  game->getGraphics()->add(n);

  if (tn) {
    tn->setValidity(false);
    tn->release();
  }

  game->getScript()->resl("s", option.data());

  return 1;
}

s32 kgmGameScript::kgmUnitSetMesh(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  kgmMesh* m = null;
  kgmMaterial* t = null;

  game->getScript()->args("ppp", &u, &m, &t);

  if (!u || !m || !t)
    return 0;

  kgmGNode* tn = static_cast<kgmGNode*>(u->getNode());
  kgmGNode* n = new kgmGNode(u, m, kgmIGraphics::NodeMesh);
  switch (t->type())
  {
    case kgmMaterial::TypePhong:
    case kgmMaterial::TypeLambert:
      n->setNodeShader(kgmIGraphics::ShaderPhong);
      break;
    case kgmMaterial::TypeBase:
    default:
      n->setNodeShader(kgmIGraphics::ShaderColor);
      n->setNodeShading(kgmIGraphics::ShadingNone);
  };

  n->setNodeMaterial(t);
  u->setNode(n);

  game->getGraphics()->add(n);

  if (tn) {
    tn->setValidity(false);
    tn->release();
  }

  return 0;
}

s32 kgmGameScript::kgmUnitAddMesh(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  const char* id = null;

  game->getScript()->args("ps", &u, &id);

  if (!u || !id)
    return 0;

  kgmMesh* m = game->getResources()->getMesh(id);

  if (m) {
    kgmGNode* tn = static_cast<kgmGNode*>(u->getNode());
    kgmGNode* n = new kgmGNode(u, m, kgmIGraphics::NodeMesh);
    u->setNode(n);

    if (tn)
        tn->release();
  }

  return 0;
}

s32 kgmGameScript::kgmUnitRemMesh(void*)
{
  return 0;
}

s32 kgmGameScript::kgmUnitSetScale(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  double x, y, z;

  game->getScript()->args("pddd", &u, &x, &y, &z);

  if (!u)
    return 0;

  vec3 v(x, y, z);

  u->scale(v);

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmUnitGetScale(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("p", &u);

  if (!u)
    return 0;

  vec3 v = u->scale();

  game->getScript()->resl("ddd", v.x, v.y, v.z);

  return 3;
}

s32 kgmGameScript::kgmUnitSetPosition(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  double x, y, z;

  game->getScript()->args("pddd", &u, &x, &y, &z);

  if (!u)
    return 0;

  vec3 v(x, y, z);

  u->position(v);

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmUnitGetPosition(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("p", &u);

  if (!u)
    return 0;

  vec3 v = u->position();

  game->getScript()->resl("ddd", v.x, v.y, v.z);

  return 3;
}

s32 kgmGameScript::kgmUnitSetRotation(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;
  double x, y, z;

  game->getScript()->args("pddd", &u, &x, &y, &z);

  if (!u)
    return 0;

  vec3 v(x, y, z);

  u->rotation(v);

  game->getScript()->resl("i", 1);

  return 1;
}

s32 kgmGameScript::kgmUnitGetRotation(void*)
{
  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  kgmUnit* u = null;

  game->getScript()->args("p", &u);

  if (!u)
    return 0;

  vec3 v = u->rotation();

  game->getScript()->resl("ddd", v.x, v.y, v.z);

  return 3;
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
    kgmArray<u8> mem;
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
    return 0;

  kgmGameScript* gs = (kgmGameScript*) game->getScript()->getX();

  game->getScript()->args("ps", &gui, &fn);

  if (gui && fn)
   gs->setSlot(gui, fn);

  return 0;
}

s32 kgmGameScript::kgmGuiCreate(void*)
{
  kgmGui* gui = null;
  s32 w, h;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

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
    return 0;

  kgmGuiContainer* g = null;
  char* t = null;
  char* id = null;
  game->getScript()->args("pssi", &g, &t, &id, &w);

  if (!g)
    return 0;

  kgmString type(t);

  if (type == "") {
    g->add(null, (kgmGuiContainer::CellSize) w, (kgmGuiContainer::Align) 0);
  } else if (type == "Label") {
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
    return 0;

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
    return 0;

  kgmGuiContainer* g = null;
  game->getScript()->args("p", &g);

  if (!g)
    return 0;

  g->setWindow(game->getWindow());

  return 0;
}

s32 kgmGameScript::kgmGcSetBgColor(void*)
{
  kgmGui* gui  = null;

  kgmIGame* game = kgmGameApp::gameApplication()->game();

  if (!game)
    return 0;

  double r, g, b;

  game->getScript()->args("ddd", &r, &g, &b);

  kgmGameGraphics* gc = static_cast<kgmGameGraphics*>(game->getGraphics());

  if (!gc)
    return 0;

  vec4 v(r, g, b, 1.0);

  gc->setBgColor(v);

  game->getScript()->resl("i", 1);

  return 1;
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
