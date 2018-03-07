#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmLog.h"

#include "../kgmMedia/kgmIAudio.h"
#include "../kgmMedia/kgmSound.h"

#include "../kgmSystem/kgmSystem.h"
#include "../kgmSystem/kgmThread.h"
#include "../kgmSystem/kgmOGL.h"
#include "../kgmSystem/kgmOAL.h"
#include "../kgmSystem/kgmOSL.h"

#include "kgmIGame.h"
#include "kgmGameMap.h"
#include "kgmGameBase.h"
#include "kgmGamePhysics.h"
#include "kgmGameResources.h"

#include "kgmUnit.h"
#include "kgmActor.h"

#include "../kgmGraphics/kgmGuiTab.h"
#include "../kgmGraphics/kgmGraphics.h"

#include "objects/kgmCar.h"
#include "objects/kgmObjects.h"
#include "objects/kgmCharacter.h"

#include "actions/kgmGuiActions.h"
#include "actions/kgmBaseActions.h"

/////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

kgmGameBase* kgmGameBase::m_game;

kgmIGame* kgmIGame::getGame()
{
  return kgmGameBase::m_game;
}

#define BWIDTH  640
#define BHEIGHT 480

//const char *log_file = "kgmLog.txt";
//const char *set_file = "kgmEngine.conf";

int   width  = 0;
int   height = 0;
float wcoef = 1.0f;
float hcoef = 1.0f;

char str_buf[1024];

u32 cmd_input = 0;

int   cc_value = 0;
float m_camDistance = 10.0f;

bool m_quit = false;
static int prev_width = 1, prev_height = 1;

kgmCString c_map;

kgmCamera* g_cam = null;
////////////////////////////////////
//                kgmGameBase
kgmGameBase::kgmGameBase(bool edit)
  :kgmWindow(0, "kgmGameWindow", 0, 0, BWIDTH, BHEIGHT, 24, false)
{
  m_game = this;

  m_resources = null;
  m_physics   = null;
  m_graphics  = null;
  m_system    = null;
  m_logic     = null;
  m_audio     = null;

  m_font = null;

  m_fps = 1;

  prev_width  = BWIDTH;
  prev_height = BHEIGHT;

  log("open settings...");
  m_settings = new kgmGameSettings();

  log("open system...");
  initSystem();

  log("init game audio...");
  initAudio();

  log("open graphics...");
  initGC();

  log("init resources...");
  initResources();

  log("init physics...");
  initPhysics();

  log("open renderer...");
  m_graphics = new kgmGameGraphics(m_gc, m_resources);
  //m_graphics->resize(m_width, m_height);

  log("init game logic...");
  initLogic();

  log("open font...");
  m_font = m_resources->getFont((char*)"font.tga", 16, 16);

  if(!m_font)
    log("can't load font");
  else if(m_graphics)
    m_graphics->setDefaultFont(m_font);

  log("set input map...");
  memset(m_keys, 0, sizeof(m_keys));

  for(u32 i = 0; i < sizeof(m_keymap); i++)
    m_keymap[i] = 0;

  for(u32 i = 0; i < sizeof(m_input); i++)
    m_input[i] = 0;

  m_keymap[KEY_LEFT]    = m_keymap[KEY_A] = (char)gbtn_left;
  m_keymap[KEY_RIGHT]   = m_keymap[KEY_D] = (char)gbtn_right;
  m_keymap[KEY_UP]      = m_keymap[KEY_W] = (char)gbtn_up;
  m_keymap[KEY_DOWN]    = m_keymap[KEY_S] = (char)gbtn_down;
  m_keymap[KEY_ESCAPE]  = (char)gbtn_exit;
  m_keymap[KEY_ENTER]   = (char)gbtn_start;
  m_keymap[KEY_LSHIFT]  = (char)gbtn_n;
  m_keymap[KEY_RSHIFT]  = (char)gbtn_n;

  m_keymap[KEY_X] = (char)gbtn_x;
  m_keymap[KEY_Y] = (char)gbtn_y;
  m_keymap[KEY_Z] = (char)gbtn_z;

  m_keymap[KEY_MSBLEFT]   = (char)gbtn_a;
  m_keymap[KEY_MSBRIGHT]  = (char)gbtn_b;
  m_keymap[KEY_MSBMIDDLE] = (char)gbtn_c;

  m_state    = State_Idle;

  /*m_islogic = true;
  m_isphysics = true;
  m_thLogic.exec((int (*)(void*))doLogic, this);
  m_thPhysics.exec((int (*)(void*))doPhysics, this);*/

  kgmGuiActions::register_actions();
  kgmBaseActions::register_actions();

#ifdef EDITOR
  if(edit)
    editor = new kEditor(this);
#endif

  int rc[4];

  getRect(rc[0], rc[1], rc[2], rc[3]);

  onResize(rc[2], rc[3]);
}

kgmGameBase::kgmGameBase(kgmString &conf)
  :kgmWindow(null, (char*)"kgmGameWindow", 0, 0, 640, 480, 24, false)
{
}

kgmGameBase::~kgmGameBase()
{
  /*m_islogic = false;
  m_isphysics = false;
  m_thLogic.join();
  m_thPhysics.join();*/

#ifdef EDITOR
  log("free editor...");

  if(editor)
    delete editor;
#endif

  log("free scene...");

  gUnload();

  log("free logic...");

  if(m_logic)
    delete m_logic;

  log("free physics...");

  if(m_physics)
    delete m_physics;

  log("free renderer...");

  if(m_graphics)
    delete m_graphics;

  log("free gui...");

  for(kgmList<kgmGui*>::iterator i = m_guis.begin(); !i.end(); ++i)
    delete (*i);

  m_guis.clear();

  log("free resources...");

  if(m_resources)
    delete m_resources;

  log("free audio...");

  if(m_audio)
    delete m_audio;

  log("free graphic context...");

  if (m_gc)
    delete m_gc;

  log("free system...");

  if(m_system)
    delete m_system;

  log("free settings...");

  if(m_settings)
    delete m_settings;

  kgmActor::g_actions.clear();
}

kgmIGC* kgmGameBase::getGC()
{
  return m_game->m_gc;
}

kgmIPhysics* kgmGameBase::getPhysics()
{
  return m_game->m_physics;
}

kgmISpawner* kgmGameBase::getSpawner()
{
  return null;
}

kgmIAudio*  kgmGameBase::getAudio()
{
  return (kgmIAudio*)m_game->m_audio;
}

kgmIVideo*  kgmGameBase::getVideo()
{
  return m_game->m_video;
}

kgmILogic*  kgmGameBase::getLogic()
{
  return m_game->m_logic;
}

kgmIGraphics*  kgmGameBase::getGraphics()
{
  return m_game->m_graphics;
}

kgmIResources* kgmGameBase::getResources()
{
  return m_game->m_resources;
}

kgmSystem*  kgmGameBase::getSystem()
{
  return m_game->m_system;
}

kgmWindow*  kgmGameBase::getWindow()
{
  return (kgmWindow*)this;
}

void kgmGameBase::initResources()
{
  m_resources = new kgmGameResources(getGC(), getAudio());

  m_resources->addPath(m_settings->get((char*)"Data"));
}

void kgmGameBase::initGraphycs()
{

}

void kgmGameBase::initPhysics()
{
  m_physics = new kgmGamePhysics();
}

void kgmGameBase::initSystem()
{
  m_system = new kgmSystem();

  m_system->getDesktopDimension(m_width, m_height);
}

void kgmGameBase::initAudio()
{
  m_audio = new kgmGameAudio();
}

void kgmGameBase::initLogic()
{
  m_logic = new kgmGameLogic();
}

void kgmGameBase::initGC()
{
  m_gc = new kgmOGL(this);
}

void kgmGameBase::log(const char* msg)
{
  kgmLog::log(msg);
}

void kgmGameBase::onIdle()
{
  static int tick = kgmTime::getTicks();
  static int frames = 0;

  s32 m_maxFps = 60;

  if(kgmTime::getTicks() - tick > 1000)
  {
    m_fps = frames;
    frames = 1;
    tick =  kgmTime::getTicks();
  }
  else
  {
    frames++;

    if(frames > m_maxFps)
    {
      s32 pause = 1000 - (kgmTime::getTicks() - tick);

      if(pause > 0)
      {
        kgmThread::sleep(pause);
      }
    }
  }

  switch(m_state)
  {
  case State_Play:
  case State_Edit:
      m_threader_1.add((kgmGameThreader::THREADER_FUNCTION)kgmGameBase::doLogic, this);
      m_threader_1.add((kgmGameThreader::THREADER_FUNCTION)kgmGameBase::doPhysics, this);
      m_threader_1.ready();
    break;
  }

  if(m_graphics)
    m_graphics->render();

  for(int i = m_guis.size(); i > 0; i--)
  {
    kgmGui* gui = m_guis[i - 1];

    if(gui->erased())
    {
      getRender()->remove(gui);

      delete gui;

      m_guis.erase(i - 1);
    }
  }

#ifdef EDITOR
  if(editor)
    editor->onIdle();
#endif
}

void kgmGameBase::onPaint()
{
}

void kgmGameBase::onClose()
{
  kgmWindow::onClose();
}

void kgmGameBase::onKeyUp(int k)
{
  m_keys[k] = 0;

  if(m_logic && (m_state == State_Play || m_state == State_Edit) && m_input[(u32) m_keymap[k]] != 0)
  {
    m_logic->input(m_keymap[k], 0);
    m_input[(u32) m_keymap[k]] = 0;
  }

#ifdef WIN32
  if(k == KEY_F11)
#else
  if(k == KEY_F12)
#endif
  {
    if(this->m_fs)
      fullscreen(false);
    else
      fullscreen(true);
  }
}

void kgmGameBase::onKeyDown(int k){
  m_keys[k] = 1;

  if(m_logic && (m_state == State_Play || m_state == State_Edit) && (m_input[(u32) m_keymap[k]] != 1))
  {
    m_logic->input(m_keymap[k], 1);
    m_input[(u32) m_keymap[k]] = 1;
  }
}

void kgmGameBase::onMsLeftUp(int k, int x, int y){
  if(m_logic && (m_state == State_Play) && (m_input[(u32) m_keymap[KEY_MSBLEFT]] != 0))
  {
    m_logic->input(m_keymap[KEY_MSBLEFT], 0);
    m_input[(u32) m_keymap[KEY_MSBLEFT]] = 0;
  }
}

void kgmGameBase::onMsLeftDown(int k, int x, int y){
  if(m_logic && (m_state == State_Play) && (m_input[(u32) m_keymap[KEY_MSBLEFT]] != 1))
  {
    m_logic->input(m_keymap[KEY_MSBLEFT], 1);
    m_input[(u32) m_keymap[KEY_MSBLEFT]] = 1;
  }
}

void kgmGameBase::onMsRightUp(int k, int x, int y){
  if(m_logic && (m_state == State_Play) && (m_input[(u32) m_keymap[KEY_MSBRIGHT]] != 0))
  {
    m_logic->input(m_keymap[KEY_MSBRIGHT], 0);
    m_input[(u32) m_keymap[KEY_MSBRIGHT]] = 0;
  }
}

void kgmGameBase::onMsRightDown(int k, int x, int y)
{
  if(m_logic && (m_state == State_Play) && (m_input[(u32) m_keymap[KEY_MSBRIGHT]] != 1))
  {
    m_logic->input(m_keymap[KEY_MSBRIGHT], 1);
    m_input[(u32) m_keymap[KEY_MSBRIGHT]] = 1;
  }
}

void kgmGameBase::onMsMove(int k, int x, int y)
{
  m_input[grot_x] = x;
  m_input[grot_y] = y;

  if(m_logic && m_state == State_Play)
  {
    m_logic->input(grot_x, x);
    m_logic->input(grot_y, y);
  }
}

void kgmGameBase::onMsWheel(int k, int x, int y, int z)
{
  m_input[grot_x] = x;
  m_input[grot_y] = y;
  m_input[grot_z] = z;

  if(m_logic && m_state == State_Play)
  {
    m_logic->input(grot_z, z);
  }
}

void kgmGameBase::onResize(int w, int h)
{
  kgmWindow::onResize(w, h);

  if(m_graphics)
    m_graphics->resize(w, h);

  float sw = (float)w / (float)prev_width;
  float sh = (float)h / (float)prev_height;
  for(int i = 0; i < m_guis.size(); i++)
    m_guis[i]->scale(sw, sh);
}

void kgmGameBase::onEvent(kgmEvent::Event* e)
{
  kgmWindow::onEvent(e);

  for(int i = m_guis.size(); i > 0; i--)
  {
    kgmGui* gui = m_guis[i - 1];

    if(!gui->erased())
      gui->onEvent(e);
  }

#ifdef EDITOR
  if(editor)
    editor->onEvent(e);
#endif
}

//Game Functions
int kgmGameBase::gLoad(kgmString s)
{
  gUnload();

#ifdef DEBUG
  kgm_log() << kgm_log_label() << " " << "Loading game map " << (char*)s << "..." << "\n";
#endif

  m_state = State_Load;

  //loadXml(s);
  {
    kgmGameMap map(this);

    kgmMemory<u8> mem;

    if(!kgmIGame::getGame()->getResources()->getFile(s, mem))
    {
      return false;
    }

    kgmXml xml;

    if(kgmXml::XML_ERROR == xml.open(mem))
    {
      return false;
    }

    map.open(xml);

    while(kgmUnit* u = map.next())
    {
      m_units.add(u);

      m_logic->add(u);
      m_graphics->add(u);
    }
  }

  if(m_logic)
    m_logic->build();

  if(m_graphics)
    m_graphics->build();

  if(m_physics)
    m_physics->build();

  m_state = State_Play;

  return m_state;
}

int kgmGameBase::gUnload()
{
  m_state = State_Clean;

  if(m_logic)
    m_logic->clear();

  if(m_physics)
    m_physics->clear();

  if(m_graphics)
    m_graphics->clean();

  for(kgmList<kgmUnit*>::iterator i = m_units.begin(); !i.end(); ++i)
    delete (*i);

  for(kgmList<kgmObject*>::iterator i = m_objects.begin(); !i.end(); ++i)
    delete (*i);

  m_units.clear();
  m_objects.clear();

  m_state = State_Idle;

  return m_state;
}

int kgmGameBase::gQuit()
{
  m_state = State_Quit;
  m_quit = true;
  m_state = State_Idle;

  close();

  return 1;
}

int kgmGameBase::gSwitch(u32 state)
{
  m_state = state;

  return m_state;
}

int kgmGameBase::gButton(game_button gb)
{
  if(gb >= sizeof(m_input))
    return 0;

  return m_input[(int)gb];
}

u32 kgmGameBase::gState(){
  return m_state;
}

void kgmGameBase::gPause(bool s){
  if(s && m_state == State_Play)
  {
    m_state = State_Pause;
  }
  else if(!s && m_state == State_Pause)
  {
    m_state = State_Play;
  }
}

bool kgmGameBase::gAppend(kgmUnit* node)
{
  if(!node)
    return false;

  if(m_graphics)
    m_graphics->add(node);

  if(m_physics && node->body())
    m_physics->add(node->body());

  if(m_logic)
  {
    if(node->isClass(kgmActor::cClass()))
      m_logic->add((kgmActor*)node);
    else if(node->isClass(kgmSensor::cClass()))
      m_logic->add((kgmSensor*)node);
    else if(node->isClass(kgmTrigger::cClass()))
      m_logic->add((kgmTrigger*)node);
    else if(node->isClass(kgmUnit::cClass()))
      m_logic->add((kgmUnit*)node);
  }

  m_units.add(node);

  return true;
}

kgmUnit* kgmGameBase::gUnit(kgmString id)
{
  kgmUnit* un = null;

  kgmList<kgmUnit*>::iterator i = m_units.begin();

  for (; !i.end(); ++i)
  {
    if ((*i)->getName() == id)
    {
      un = (*i);

      if(un->removed() || !un->valid())
      {
        return null;
      }

      break;
    }
  }

  return un;
}

kgmIGame::Iterator* kgmGameBase::gObjects()
{
  GIterator* it = new GIterator(this);

  return it;
}

inline void xmlAttr(kgmXml::Node* node, const char* id, kgmString& val)
{
  kgmString sid;
  sid = id;
  node->attribute(sid, val);
}

inline void xmlAttr(kgmXml::Node* node, const char* id, int& val)
{
  kgmString sid, v;
  sid = id;
  node->attribute(sid, v);
  val = atoi(v.data());
}

inline void xmlAttr(kgmXml::Node* node, const char* id, float& val)
{
  kgmString sid, v;
  sid = id;
  node->attribute(sid, v);
  val = atof(v.data());
}

inline void xmlAttr(kgmXml::Node* node, const char* id, vec3& val)
{
  kgmString sid, v;
  float     a[3];
  sid = id;
  node->attribute(sid, v);
  sscanf(v.data(), "%f %f %f", &a[0], &a[1], &a[2]);
  val = vec3(a[0], a[1], a[2]);
}

inline void xmlAttr(kgmXml::Node* node, const char* id, quat& val)
{
  kgmString sid, v;
  float     a[4];
  sid = id;
  node->attribute(sid, v);
  sscanf(v.data(), "%f %f %f %f", &a[0], &a[1], &a[2], &a[3]);
  val = quat(a[0], a[1], a[2], a[3]);
}

//
// Load XML and fill objects
//

bool kgmGameBase::loadXml(kgmString& path)
{
#define TypeNone       0
#define TypeMaterial   1
#define TypeCamera     2
#define TypeLight      3
#define TypeMesh       4
#define TypeActor      5
#define TypeCollision  6
#define TypeGameObject 7

#define AttrString(node, id, val) \
  { \
  kgmString sid; \
  sid = id; \
  node->attribute(sid, val); \
  }

  kgmMemory<u8> mem;

  if(!kgmIGame::getGame()->getResources()->getFile(path, mem))
  {
#ifdef DEBUG
    kgm_log() << "\nCan't find map " << (char*)path;
#endif

    return false;
  }

  kgmXml xml;

  if(kgmXml::XML_ERROR == xml.open(mem))
  {
#ifdef DEBUG
    kgm_log() << "\nNot valid xml %s" << (char*)path;
    kgm_log() << "Error: " << "Not valid xml " << (char*)path << "\n";
#endif

    return false;
  }

  u32             type = TypeNone;
  kgmUnit*        gob = 0;

  kgmMesh*        msh = 0;
  kgmLight*       lgt = 0;
  kgmMaterial*    mtl = 0;
  kgmActor*       act = 0;
  kgmVisual*      vis = 0;

  u32             vts = 0;

  kgmString      m_data = "";

  while(kgmXml::XmlState xstate = xml.next())
  {
    kgmString id, value, t;

    if(xstate == kgmXml::XML_ERROR)
    {
      break;
    }
    else if(xstate == kgmXml::XML_FINISH)
    {
      break;
    }
    else if(xstate == kgmXml::XML_TAG_OPEN)
    {
      id = xml.m_tagName;

#ifdef DEBUG
      kgm_log() << "Node: " << (char*)id << "\n";
#endif

      if(id == "Material")
      {
        kgmString id;
        xml.attribute("name", id);
        type = TypeMaterial;

        mtl = new kgmMaterial();

        m_objects.add(mtl);
      }
      else if(id == "Camera")
      {
        type = TypeCamera;
      }
      else if(id == "Light")
      {
        type = TypeLight;

        lgt = new kgmLight();
        gob = new kgmUnit(this, lgt);

        m_graphics->add((kgmIGraphics::INode*) gob);
      }
      else if(id == "Mesh")
      {
        type = TypeMesh;

        msh = new kgmMesh();
        gob = new kgmUnit(this, msh);

        m_units.add(gob);
        m_graphics->add((kgmIGraphics::INode*) gob);
      }
      else if(id == "Actor")
      {
        type = TypeActor;
        kgmString s, sgrp;

        xml.attribute("object", s);
        xml.attribute("group", sgrp);

        gob = act = (kgmActor*) gSpawn(s);

        if(act)
        {
          xml.attribute("name", s);
          act->setName(s);
          xml.attribute("player", s);

          if(s == "on")
            act->m_gameplayer = true;

          if(sgrp.length() > 0)
            act->setGroup(kgmConvert::toInteger(sgrp));

          gAppend((kgmActor*)act);
        }
      }
      else if(id == "Unit")
      {
        type = TypeGameObject;
        kgmString s, sgrp;

        xml.attribute("object", s);
        xml.attribute("group", sgrp);

        gob = gSpawn(s);

        if(sgrp.length() > 0)
          gob->setGroup(kgmConvert::toInteger(sgrp));

        gAppend((kgmUnit*)gob);
      }
      else if(id == "Vertices")
      {
        int len = 0;

        xml.attribute("length", value);
        sscanf(value.data(), "%i", &len);
        msh->vAlloc(len, kgmMesh::FVF_P_N_C_T);
        m_data = "vertices";
      }
      else if(id == "Faces")
      {
        int len = 0;

        xml.attribute("length", value);
        sscanf(value.data(), "%i", &len);
        msh->fAlloc(len, kgmMesh::FFF_16);
        m_data = "faces";
      }
      else if(id == "Polygon")
      {
        xml.attribute("vertices", value);
        sscanf(value.data(), "%i", &vts);
        m_data = "polygon";
      }
    }
    else if(xstate == kgmXml::XML_TAG_CLOSE)
    {
      kgmString data;
      id = xml.m_tagName;

#ifdef DEBUG
      kgm_log() << "Node: " << (char*)id << "\n";
#endif

      if(id == "Color")
      {
      }
      else if(id == "Ambient")
      {
      }
      else if(id == "Specular")
      {
      }
      else if(id == "Shininess")
      {
      }
      else if(id == "Alpha")
      {
        if(xml.attribute("value", data))
        {
          float a;

          sscanf(data, "%f", &a);

          if(a < 1.0f)
          {
            mtl->alpha(true);
          }
        }
      }
      else if(id == "Cull")
      {
        mtl->cull(true);
      }
      else if(id == "map_color")
      {
        if(xml.attribute("value", data))
        {
          mtl->setTexColor(m_resources->getTexture(data));
        }
      }
      else if(id == "map_normal")
      {
        kgmString data;

        if(xml.attribute("value", data))
        {
          mtl->setTexNormal(m_resources->getTexture(data));
        }
      }
      else if(id == "map_specular")
      {
        kgmString data;

        if(xml.attribute("value", data))
        {
          mtl->setTexSpecular(m_resources->getTexture(data));
        }
      }
      else if(id == "Shader")
      {
        kgmString data;

        if(xml.attribute("value", data))
        {
          mtl->setShader(m_resources->getShader(value));
        }
      }
      else if(id == "Position")
      {
        vec3 v;
        xml.attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        switch(type){
        case TypeCamera:
          m_graphics->camera().mPos = v;
          break;
        default:
          gob->position(v);
          break;
        }
      }
      else if(id == "Rotation")
      {
        vec3 v;
        xml.attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        if(act && type == TypeActor)
        {
          act->rotation(v);
        }
        else if(type == TypeLight)
        {

        }
      }
      else if(id == "Quaternion")
      {
        quat q;
        xml.attribute("value", value);
        sscanf(value.data(), "%f %f %f %f", &q.x, &q.y, &q.z, &q.w);

        if(act && type == TypeActor)
          act->quaternion(q);
      }
      else if(id == "State")
      {
        kgmString s;
        xml.attribute("value", s);

        if(act && type == TypeActor)
          act->setState(s);
      }
      else
      {
        kgmString s;

        if(act && type == TypeActor && xml.attribute("value", s))
          act->setOption(id, s);
      }
    }
    else if(xstate == kgmXml::XML_TAG_DATA)
    {
      if(m_data == "vertices")
      {
        int n = 0;
        char* pdata = xml.m_tagData.data();
        kgmMesh::Vertex_P_N_C_T* v = (kgmMesh::Vertex_P_N_C_T*)msh->vertices();

        for (u32 i = 0; i < msh->vcount(); i++) {
          sscanf(pdata, "%f %f %f %f %f %f %f %f%n",
                 &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
                 &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
                 &v[i].uv.x, &v[i].uv.y, &n);
          v[i].col = 0xffffffff;
          (pdata) += (u32)n;
        }

        m_data = "";
      }
      else if(m_data == "faces")
      {
        int n = 0;
        char* pdata = xml.m_tagData.data();
        kgmMesh::Face_16* f = (kgmMesh::Face_16*)msh->faces();

        for (u32 i = 0; i < msh->fcount(); i++) {
          u32 fs[4];
          sscanf(pdata, "%i %i %i %n", &fs[0], &fs[1], &fs[2], &n);
          f[i].f[0] = fs[0];
          f[i].f[1] = fs[1];
          f[i].f[2] = fs[2];
          (pdata) += (u32)n;
        }

        m_data = "";
      }
      else if(m_data == "skin")
      {
        m_data = "";
      }
      else if(m_data == "polygon")
      {
        int n = 0;
        char* pdata = xml.m_tagData.data();

        if(vts > 2)
        {
          vec3* v = new vec3[vts];

          for(u32 i = 0; i < vts; i++)
          {
            sscanf(pdata, "%f %f %f %n", &v[i].x, &v[i].y, &v[i].z, &n);
            (pdata) += (u32)n;
          }

          for(u32 i = 1; i < vts - 1; i++)
          {
            m_physics->add(v[0], v[i], v[i+1]);
          }

          delete [] v;
        }

        m_data = "";
      }
    }
  }

  return true;
}

kgmUnit* kgmGameBase::gSpawn(kgmString a)
{
  kgmActor*       actor = 0;
  kgmString       type = a;
  kgmMemory<u8>   mem;

#ifdef DEBUG
  kgm_log() << "\nSpawning Actor: " << a.data();
#endif

  if(!m_resources->getFile(a, mem))
  {
    if(!m_resources->getFile(a += ".act", mem))
    {
      if(!m_resources->getFile(a += ".kgm", mem))
      {
        return new kgmUnit();
      }
    }
  }

  kgmXml xml(mem);

  if(!xml.m_node)
    return 0;

  kgmXml::Node* a_node = null;

  for(int i = 0; i < xml.m_node->nodes(); i++)
  {
    kgmString id;
    xml.m_node->node(i)->id(id);

    if(id == "kgmActor")
    {
      a_node = xml.m_node->node(i);

      break;
    }
  }

  if(!a_node)
    return null;

  kgmString stype;

  a_node->attribute("type", stype);

  actor = new kgmActor(this);

  if(!actor)
    return null;

  for(int i = 0; i < a_node->nodes(); ++i){
    kgmString id, val;

    if(a_node->node(i))
      a_node->node(i)->id(id);
    else
      break;

    if(id == "Mass")
    {
      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f", &actor->body()->m_mass);
    }
    else if(id == "Bound")
    {
      float a[3];
      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f%f%f", &a[0], &a[1], &a[2]);
      actor->body()->m_bound.min = vec3(-0.5 * a[0], -0.5 * a[1], 0.0);
      actor->body()->m_bound.max = vec3( 0.5 * a[0],  0.5 * a[1], a[2]);
    }
    else if(id == "Collision")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "convex")
      {
        kgmMemory<u8> mem;
        kgmString     dfile;

        a_node->node(i)->attribute("data", dfile);

        if(m_resources->getFile(dfile, mem))
        {
          kgmXml xml(mem);

          if(xml.m_node)
          {
            for(int i = 0; i < xml.m_node->nodes(); i++)
            {
              kgmXml::Node* node = xml.m_node->node(i);

              if(node->m_name == "kgmCollision")
              {
                node->attribute("polygons", val);

                for(int j = 0; j < xml.m_node->node(i)->nodes(); j++)
                {
                  kgmXml::Node* node = xml.m_node->node(i)->node(j);

                  if(node->m_name == "Polygon")
                  {
                    kgmString scount;
                    u32       count;

                    node->attribute("vertices", scount);
                    count = kgmConvert::toInteger(scount);

                    vec3*     pol = new vec3[count];
                    vec3      v;
                    int       n = 0;
                    char*     pdata = node->m_data.data();

                    for(u32 k = 0; k < count; k++)
                    {
                      sscanf(pdata, "%f %f %f%n", &v.x, &v.y, &v.z, &n);
                      (pdata) += (u32)n;
                      pol[k] = v;
                    }

                    for(u32 k = 2; k < count; k++)
                    {
                      vec3 v[3] = {pol[0], pol[k - 1], pol[k]};

                      actor->body()->addShapeSide(v);
                    }

                    delete [] pol;
                  }
                }
              }
            }
          }
        }

        if(actor->body()->m_convex.size() > 0)
          actor->body()->m_shape = (u32) kgmBody::ShapePolyhedron;
      }
    }
    else if(id == "Gravity")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "true")
        actor->body()->m_gravity = true;
      else
        actor->body()->m_gravity = false;
    }
    else if(id == "Dummies")
    {
      kgmMemory<u8> mem;
      a_node->node(i)->attribute("value", val);

      if((val.length() > 0) && m_resources->getFile(val, mem))
      {
        kgmXml xml(mem);

        if(xml.m_node)
        {
          for(int i = 0; i < xml.m_node->nodes(); i++)
          {
            kgmXml::Node* node = xml.m_node->node(i);

            if(!node)
              break;

            if(node->m_name == "kgmDummy")
            {
              kgmDummy* dummy = new kgmDummy();

              actor->add(dummy);
              //              dummy->release();
              node->attribute("name", dummy->m_id);

              for(int j = 0; j < node->nodes(); j++)
              {
                kgmXml::Node* jnode = node->node(j);

                if(!jnode)
                  break;

                if(jnode->m_name == "Position")
                {
                  kgmString spos;
                  vec3      v;

                  jnode->attribute("value", spos);
                  sscanf(spos, "%f %f %f", &v.x, &v.y, &v.z);
                  dummy->setShift(v);
                }
                else if(jnode->m_name == "Rotation")
                {
                  kgmString srot;
                  vec3      r;

                  jnode->attribute("value", srot);
                  sscanf(srot, "%f %f %f", &r.x, &r.y, &r.z);
                  dummy->setOrient(r);
                }
              }
            }
          }

          xml.close();
        }

        mem.clear();
      }
    }
    else if(id == "GameObject" || id == "Actor")
    {
      kgmString cid, type, dummy;
      bool isactor = (id == "Actor") ? (true) : (false);

      a_node->node(i)->attribute("id",    cid);
      a_node->node(i)->attribute("type",  type);
      a_node->node(i)->attribute("dummy", dummy);

      kgmUnit* go = gSpawn(type);

      if(go)
      {
        go->setName(cid);

        kgmDummy* dm = actor->dummy(dummy);

        if(dm)
          dm->attach(go, kgmDummy::AttachToObject);

        gAppend((kgmUnit*)go);
      }
    }
    else if(id == "Visual")
    {
      kgmMesh*      msh = 0;
      kgmMaterial*  mtl = 0;
      kgmSkeleton*  skl = 0;
      kgmAnimation* anm = 0;

      for(int j = 0; j < a_node->node(i)->nodes(); j++)
      {
        if(a_node->node(i)->node(j))
          a_node->node(i)->node(j)->id(id);
        else
          break;

        if(id == "Material"){
          a_node->node(i)->node(j)->attribute("value", val);
          mtl = null; //m_resources->getMaterial(val);
        }else if(id == "Mesh"){
          a_node->node(i)->node(j)->attribute("value", val);
          msh = m_resources->getMesh(val);
        }else if(id == "Animation"){
          a_node->node(i)->node(j)->attribute("value", val);
          anm = m_resources->getAnimation(val);
        }else if(id == "Skeleton"){
          a_node->node(i)->node(j)->attribute("value", val);
          skl = m_resources->getSkeleton(val);
        }else if(id == "Dummy"){
          a_node->node(i)->node(j)->attribute("value", val);
        }
      }

      if(msh)
      {
        //actor->visual()->set(msh);
        //actor->visual()->set(mtl);
        //actor->visual()->setAnimation(anm);
        //actor->visual()->setSkeleton(skl);

        msh = null;
        mtl = null;
        skl = null;
        anm = null;
      }
    }
    else if(id == "Input")
    {
      u32 btn = 0, btn1 = 0, btn2 = 0, stat = 0;
      kgmString state;

      a_node->node(i)->attribute("button",  val);  sscanf(val, "%i", &btn);
      val = "";
      a_node->node(i)->attribute("button1", val);  sscanf(val, "%i", &btn1);
      val = "";
      a_node->node(i)->attribute("button2", val);  sscanf(val, "%i", &btn2);
      val = "";
      a_node->node(i)->attribute("status",  val);  sscanf(val, "%i", &stat);
      a_node->node(i)->attribute("state",   state);

      actor->add(btn, stat, state, btn1, btn2);
    }
    else if(id == "InputActive")
    {
      u32 btn = 0, btn1 = 0, btn2 = 0, stat = 1;
      kgmString state;

      a_node->node(i)->attribute("button",  val);  sscanf(val, "%i", &btn);
      val = "";
      a_node->node(i)->attribute("button1", val);  sscanf(val, "%i", &btn1);
      val = "";
      a_node->node(i)->attribute("button2", val);  sscanf(val, "%i", &btn2);
      a_node->node(i)->attribute("state",   state);

      actor->add(btn, stat, state, btn1, btn2, true);
    }
    else if(id == "State")
    {
      kgmString s;
      kgmActor::State* state = new kgmActor::State();

      a_node->node(i)->attribute("id", state->id);
      a_node->node(i)->attribute("type", state->type);
      a_node->node(i)->attribute("switch", state->switchto);

      a_node->node(i)->attribute("time", s);
      if(s.length() > 0) sscanf(s, "%i", &state->timeout);

      a_node->node(i)->attribute("priority", s);
      if(s.length() > 0) sscanf(s, "%i", &state->priopity);

      for(int j = 0; j < a_node->node(i)->nodes(); j++)
      {
        if(a_node->node(i)->node(j))
          a_node->node(i)->node(j)->id(id);
        else
          break;

        if(id == "Sound")
        {
          a_node->node(i)->node(j)->attribute("value", s);
          state->sound = kgm_ptr<kgmSound>(m_resources->getSound(s));
        }
        else if(id == "Animation")
        {
          a_node->node(i)->node(j)->attribute("value", s);
          state->animation = m_resources->getAnimation(s);
          a_node->node(i)->node(j)->attribute("start", s);
          if(s.length() > 0) sscanf(s, "%i", &state->fstart);
          a_node->node(i)->node(j)->attribute("end", s);
          if(s.length() > 0) sscanf(s, "%i", &state->fend);
        }
        else if(id == "Option")
        {
          kgmString k, v;
          a_node->node(i)->node(j)->attribute("key", k);
          a_node->node(i)->node(j)->attribute("value", v);
          //state->options.add(k,v);
        }
      }
      actor->m_states.add(state);
    }
    else
    {
      kgmString val;

      if(a_node->node(i)->attribute("value", val))
      {
        actor->m_options.set(id, val);
      }
    }
  }

#ifdef DEBUG
  kgm_log() << "\nActor: " << actor->getName();
#endif

  return actor;
}

int kgmGameBase::doLogic(kgmGameBase* g)
{
  //while(g->m_islogic && g->m_logic)
  {
    u32 fps = 1 + g->m_fps;
    g->m_logic->update(1000 / fps);
    //kgmThread::sleep(0);
  }

  return 1;
}

int kgmGameBase::doPhysics(kgmGameBase* g)
{
  g->m_physics->update();

  return 1;
}
