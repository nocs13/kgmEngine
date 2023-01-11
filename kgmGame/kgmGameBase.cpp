#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmLog.h"

#include "../kgmMedia/kgmIAudio.h"
#include "../kgmMedia/kgmSound.h"

#include "../kgmSystem/kgmSystem.h"
#include "../kgmSystem/kgmThread.h"
#include "../kgmSystem/kgmOGL.h"
#include "../kgmSystem/kgmOAL.h"
#include "../kgmSystem/kgmOSL.h"
#include "../kgmSystem/kgmTime.h"
#include "../kgmSystem/kgmVulkan.h"
#include "../kgmSystem/kgmGCNone.h"

#include "kgmIGame.h"
#include "kgmGameMap.h"
#include "kgmGameBase.h"
#include "kgmGamePhysics.h"
#include "kgmGameResources.h"

#include "kgmUnit.h"

#include "../kgmGraphics/kgmGuiTab.h"
#include "../kgmGraphics/kgmGraphics.h"

#include "objects/kgmCar.h"
#include "objects/kgmObjects.h"
#include "objects/kgmCharacter.h"

//#include "actions/kgmGuiActions.h"
//#include "actions/kgmBaseActions.h"

/////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

kgmGameBase* kgmGameBase::m_game;
bool         kgmGameBase::m_need_editor = false;

#define BWIDTH  1920
#define BHEIGHT 1080

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

//kgmCamera* g_cam = null;
////////////////////////////////////

kgmIGame::~kgmIGame()
{
#ifdef DEBUG
    kgm_log() << "kgmIGame::~kgmIGame [" << (void*)this << "]\n";
#endif
}

//                kgmGameBase
kgmGameBase::kgmGameBase()
  :kgmWindow(nullptr, "kgmGameWindow", 0, 0, BWIDTH, BHEIGHT, 24, false)
{
  m_game = this;

  m_resources = null;
  m_physics   = null;
  m_graphics  = null;
  m_system    = null;
  m_script    = null;
  m_input     = null;
  m_audio     = null;
  m_gc        = null;
  m_ai        = null;

  m_retention = null;
  m_settings  = null;

  m_messenger = null;

  m_font = null;

  m_fps = 1;
  m_tpf = 1000;

  prev_width  = BWIDTH;
  prev_height = BHEIGHT;

  int rc[4];

  getRect(rc[0], rc[1], rc[2], rc[3]);

  //onResize(rc[2], rc[3]);

  m_state = -1;
}

kgmGameBase::~kgmGameBase()
{
  log("free scene...");

  gUnload();

  log("free scripts...");

  if(m_script)
    delete m_script;

  log("free messenger...");

  if (m_messenger)
    m_messenger->release();

  log("free ai...");

  if(m_ai)
    delete m_ai;

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

  log("free input...");

  if(m_input)
    delete m_input;

  log("free system...");

  if(m_system)
    delete m_system;

  log("free settings...");

  if(m_settings)
    delete m_settings;

  if (m_retention)
    delete m_retention;

#ifdef DEBUG
    kgm_log() << "kgmGameBase::~kgmGameBase [" << (void*)this << "]\n";
#endif
}


kgmIGC* kgmGameBase::getGC()
{
  return m_game->m_gc;
}

kgmIAI* kgmGameBase::getAI()
{
  return m_game->m_ai;
}

kgmIPhysics* kgmGameBase::getPhysics()
{
  return m_game->m_physics;
}

kgmIAudio*  kgmGameBase::getAudio()
{
  return (kgmIAudio*)m_game->m_audio;
}

kgmIVideo*  kgmGameBase::getVideo()
{
  return m_game->m_video;
}

kgmIInput*  kgmGameBase::getInput()
{
  return m_game->m_input;
}

kgmIScript*  kgmGameBase::getScript()
{
  return m_game->m_script->getHandler();
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

kgmIGame::Messenger*  kgmGameBase::getMessenger()
{
  return m_messenger;
}

u32 kgmGameBase::timeUpdate()
{
  return m_tpf;
}

void kgmGameBase::initResources()
{
  m_resources = (kgmGameResources*) kgmGameResources::generate(getGC(), getAudio());

  kgmString s = m_settings->get((char*)"Data");

  if (!s.data()) {
    kgm_log() << "Error: No 'Data' in settings.";
    return;
  }

  char* tok = strtok((char*) s.data(), ":");

  while(tok) {
    m_resources->addPath(tok);
    tok = strtok(nullptr, ":");
  }
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

void kgmGameBase::initInput()
{
  m_input = new kgmGameInput(this, m_keys, sizeof(m_keys));
}

bool kgmGameBase::initScript()
{
  m_script = new kgmGameScript(this);

  m_script->init();

  if (!m_script->getStatus())
    return false;

  return true;
}

void kgmGameBase::initAI()
{
  m_ai = new kgmGameAI(this);
}

void kgmGameBase::initGC()
{
  kgmString v = m_settings->get((char*)"GC");

  if (v.length() < 1) {
    v = "OGL";
  }

  kgm_log() << "Choosed GC is " << (char*) v << ".\n";

  if (v == "VK") {
    #ifndef DARWIN
    m_gc = new kgmVulkan(this);

    if(m_gc->gcError()) {
      kgm_log() << "Vulkan initialization failed.\n";
      delete m_gc;
      
      m_gc = nullptr;
    }
    #endif
  } else if (v == "OGL") {
    m_gc = new kgmOGL(this);
  } else {
    //m_gc = new kgmGCNone(this);
  }
}

void kgmGameBase::initSettings()
{
  m_settings = new kgmGameSettings();
}

void kgmGameBase::log(const char* msg)
{
  kgm_log() << msg << "\n";
}

void kgmGameBase::onIdle()
{
  static int tick = kgmTime::getTicks();
  static int frames = 1;

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

  m_tpf = 1000 / m_fps;

  switch(m_state)
  {
  case State_Play:
  case State_Edit:
    break;
  }

  if(m_graphics)
    m_graphics->render();

  for(int i = m_guis.size(); i > 0; i--)
  {
    kgmGui* gui = m_guis[i - 1];

    if(gui && gui->erased())
    {
      getRender()->remove(gui);

      delete gui;

      m_guis.erase(i - 1);
    }
  }
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
#ifdef DEBUG
  kgm_log() << "kgmGameBase::onKeyUp [" << k << "]\n";
#endif

  if (k < KEY_NONE || k > KEY_END)
    return;

  if (!m_keys[k])
    return;

  m_keys[k] = 0;

  if (m_script && (k == KEY_ESCAPE))
    m_script->onPause();
}

void kgmGameBase::onKeyDown(int k)
{
#ifdef DEBUG
  kgm_log() << "kgmGameBase::onKeyDown [" << k << "]\n";
#endif

  if (k < KEY_NONE || k > KEY_END)
    return;

  if (m_keys[k])
    return;

  m_keys[k] = 1;
}

void kgmGameBase::onMsLeftUp(int k, int x, int y)
{
  m_keymap[KEY_MSBLEFT] = 0;
}

void kgmGameBase::onMsLeftDown(int k, int x, int y)
{
  m_keymap[KEY_MSBLEFT] = 1;
}

void kgmGameBase::onMsRightUp(int k, int x, int y)
{
  m_keymap[KEY_MSBRIGHT] = 0;
}

void kgmGameBase::onMsRightDown(int k, int x, int y)
{
  m_keymap[KEY_MSBRIGHT] = 1;
}

void kgmGameBase::onMsMove(int k, int x, int y)
{
  m_point[0] = x;
  m_point[1] = y;

  if(m_input)
  {
    m_input->setX(0, x);
    m_input->setX(0, y);
  }
}

void kgmGameBase::onMsWheel(int k, int x, int y, int z)
{
  m_point[2] = z;

  if(m_input)
  {
    m_input->setZ(0, z);
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

  kgmGui* visible[4] = {0};
  s32     count = 0;

  if (e->event == evtKeyDown)
    count = 0;

  for(auto i = m_guis.begin(); !i.end(); i.next())
  {
    kgmGui* gui = (*i);

    if(gui && gui->visible() && !gui->erased())
    {
      if (count == 3)
        break;
      else
        visible[count++] = gui;
    }
  }

  for (s32 i = 0; i < count; i++)
    visible[i]->onEvent(e);

  if (m_messenger)
    m_messenger->onEvent(e);
}

void kgmGameBase::guiAdd(kgmGui* g)
{
  if(g)
  {
    for(int i = 0; i < m_guis.size(); i++)
    {
      if(g == m_guis[i])
      {
        return;
      }
    }

    m_guis.add(g);

    if(m_graphics)
      m_graphics->add(g);
  }
}

int kgmGameBase::gInit()
{
  if (m_state != -1)
    return 0;

  //m_state = State_Idle;

  log("open settings...");
  initSettings();

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
  m_graphics->resize(m_width, m_height);

  log("init game input...");
  initInput();

  if(!m_font)
  {
    m_font = m_resources->getFont("font.tga", 16, 16);

    if (!m_font)
      log("can't load font");
  }

  if(m_graphics)
    m_graphics->setDefaultFont(m_font);

  if (!m_messenger)
    m_messenger = new kgmGameMessenger(this);

  memset(m_keys, 0, sizeof(m_keys));

  m_state    = State_Idle;

  //kgmGuiActions::register_actions();
  //kgmBaseActions::register_actions();

  log("init game script...");

  if (!initScript())
    return 0;

  log("init ai...");
  initAI();

  m_state = State_Idle;

  return 1;
}

int kgmGameBase::gQuit()
{
  m_state = State_Quit;

  m_quit = true;

  gUnload();

  if (m_script)
    m_script->onQuit();

  m_state = State_Idle;

  close();

  return 1;
}

int kgmGameBase::gLoad(kgmString s)
{
  gUnload();

#ifdef DEBUG
  kgm_log() << "Loading game map " << (char*)s << "..." << "\n";
#endif

  m_state = State_Load;

  if (s == (const char*) "EMPTY")
  {
    #ifdef DEBUG
      kgm_log() << " " << "Loading empty test map." << "\n";
    #endif

    m_state = State_Play;

    return m_state;
  }

  //loadXml(s);
  {
    kgmGameMap map(this);

    kgmArray<u8> mem;

    kgmString sf = kgmString("maps");

    sf += (kgmSystem::getPathDelim() + s);

    if(!getResources()->getFile(sf, mem))
    {
      m_state = State_Idle;

      return m_state;
    }

    kgmXml xml;

    if(kgmXml::XML_ERROR == xml.open(mem))
    {
      m_state = State_Idle;

      return m_state;
    }

    map.open(xml);

    while(kgmUnit* u = map.next())
    {
      m_units.add(u);

      kgmGraphics::INode* n = u->getNode();

      if (m_graphics)
        m_graphics->add(u->getNode());

      if (m_physics)
        m_physics->add(u->getBody());
    }
  }

  if(m_graphics)
    m_graphics->build();

  if(m_physics)
    m_physics->build();

  if (m_script)
    m_script->onLoad();

  if (m_ai)
    m_ai->start();

  m_state = State_Play;

  return m_state;
}

int kgmGameBase::gUnload()
{
  m_state = State_Clean;

  if (m_ai)
    m_ai->clean();

  if(m_physics)
    m_physics->clear();

  if(m_graphics)
    m_graphics->clear();

  if (m_script)
    m_script->onUnload();

  for(kgmList<kgmUnit*>::iterator i = m_units.begin(); !i.end(); ++i)
  {
    if (m_script)
      m_script->onRemove((*i));

    (*i)->release();
  }

  for(kgmList<kgmObject*>::iterator i = m_objects.begin(); !i.end(); ++i)
    (*i)->release();

  m_units.clear();
  m_objects.clear();

  m_state = State_Idle;

  return m_state;
}

int kgmGameBase::gSwitch(u32 state)
{
  m_state = state;

  return m_state;
}

int kgmGameBase::gButton(game_button gb)
{
  return 0;
}

u32 kgmGameBase::gState()
{
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
    m_graphics->add(node->getNode());

  if(m_physics)
    m_physics->add(node->getBody());

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

kgmIGame::Iterator* kgmGameBase::gUnits()
{
  UnitIterator* it = new UnitIterator(this);

  return it;
}

bool kgmGameBase::gUnitRegister(kgmString type, NEW_UNIT fn)
{
  if (fn == NULL || type.length() < 1)
    return false;

  auto i = m_unit_generators.get(type);

  if (i.isValid() && i.data() != NULL) {

  }

  m_unit_generators.set(type, fn);

  return true;
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
#define TypeUnit       5
#define TypeCollision  6
#define TypeGameObject 7

#define AttrString(node, id, val) \
  { \
  kgmString sid; \
  sid = id; \
  node->attribute(sid, val); \
  }

  kgmArray<u8> mem;

  if(!getResources()->getFile(path, mem))
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
        gob = new kgmUnit(this);
        gob->setNode(new kgmGNode(gob, lgt, kgmIGraphics::NodeLight));

        m_graphics->add((kgmIGraphics::INode*) gob);
      }
      else if(id == "Mesh")
      {
        type = TypeMesh;

        msh = new kgmMesh();
        gob = new kgmUnit(this);

        m_units.add(gob);
        m_graphics->add((kgmIGraphics::INode*) gob);
      }
      else if(id == "Actor")
      {
        type = TypeUnit;
        kgmString s, sgrp;

        xml.attribute("object", s);
        xml.attribute("group", sgrp);

        gob = gSpawn(s);

        if(gob)
        {
          xml.attribute("name", s);
          gob->setName(s);
          xml.attribute("player", s);

          //if(s == "on")
          //  act->m_gameplayer = true;

          gAppend(gob);
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
        msh->vAlloc(len, kgmMesh::FVF_P_N_T);
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
      /*else if(id == "Alpha")
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
      }*/
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
          //mtl->setShader(m_resources->getShader(value));
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

        if(gob && type == TypeUnit)
        {
          gob->rotation(v);
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

        //if(act && type == TypeActor)
          //act->quaternion(q);
      }
      else if(id == "State")
      {
        kgmString s;
        xml.attribute("value", s);

        //if(gob && type == TypeUnit)
          //gob->setState(s);
      }
      else
      {
        kgmString s;

        if(gob && type == TypeUnit && xml.attribute("value", s))
          gob->setOption(id, s);
      }
    }
    else if(xstate == kgmXml::XML_TAG_DATA)
    {
      if(m_data == "vertices")
      {
        int n = 0;
        char* pdata = xml.m_tagData.data();
        kgmMesh::Vertex_P_N_T* v = (kgmMesh::Vertex_P_N_T*)msh->vertices();

        for (u32 i = 0; i < msh->vcount(); i++) {
          sscanf(pdata, "%f %f %f %f %f %f %f %f%n",
                 &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
                 &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
                 &v[i].uv.x, &v[i].uv.y, &n);
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
  kgmUnit*       actor = 0;
  kgmString      type = a;
  kgmArray<u8>   mem;

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

  actor = new kgmUnit(this);

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
      f32 mass;

      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f", &mass);

      actor->getBody()->bodyMass(mass);
    }
    else if(id == "Bound")
    {
      float a[3];
      box bb;

      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f%f%f", &a[0], &a[1], &a[2]);
      bb.min = vec3(-0.5 * a[0], -0.5 * a[1], 0.0);
      bb.max = vec3( 0.5 * a[0],  0.5 * a[1], a[2]);

      actor->getBody()->bodyBound(bb);
    }
    else if(id == "Collision")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "convex")
      {
        kgmArray<u8> mem;
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

                      //actor->body()->addShapeSide(v);
                    }

                    delete [] pol;
                  }
                }
              }
            }
          }
        }

        //if(actor->body()->m_convex.size() > 0)
        //  actor->body()->m_shape = (u32) kgmBody::ShapePolyhedron;
      }
    }
    else if(id == "Gravity")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "true" && actor->getBody())
        actor->getBody()->bodyGravity(true);
      else if (actor->getBody())
        actor->getBody()->bodyGravity(false);
    }
    else if(id == "Dummies")
    {
      kgmArray<u8> mem;
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

      //actor->add(btn, stat, state, btn1, btn2);
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

      //actor->add(btn, stat, state, btn1, btn2, true);
    }
    else if(id == "State")
    {
      kgmString s;
      kgmIAI::State state;

      a_node->node(i)->attribute("id", state.id);
      a_node->node(i)->attribute("switch", state.switchto);

      a_node->node(i)->attribute("time", s);
      if(s.length() > 0) sscanf(s, "%i", &state.timeout);

      a_node->node(i)->attribute("priority", s);
      if(s.length() > 0) sscanf(s, "%i", &state.priopity);

      for(int j = 0; j < a_node->node(i)->nodes(); j++)
      {
        if(a_node->node(i)->node(j))
          a_node->node(i)->node(j)->id(id);
        else
          break;

        if(id == "Sound")
        {
          a_node->node(i)->node(j)->attribute("value", s);
          state.sound = m_resources->getSound(s);
        }
        else if(id == "Animation")
        {
          a_node->node(i)->node(j)->attribute("value", s);
          state.animation = m_resources->getAnimation(s);
          a_node->node(i)->node(j)->attribute("start", s);
          if(s.length() > 0) sscanf(s, "%i", &state.fstart);
          a_node->node(i)->node(j)->attribute("end", s);
          if(s.length() > 0) sscanf(s, "%i", &state.fend);
        }
        else if(id == "Option")
        {
          kgmString k, v;
          a_node->node(i)->node(j)->attribute("key", k);
          a_node->node(i)->node(j)->attribute("value", v);
          //state->options.add(k,v);
        }
      }

      m_ai->addState(actor->getClass(), state);
    }
    else
    {
      kgmString val;

      if(a_node->node(i)->attribute("value", val))
      {
        //actor->m_options.set(id, val);
      }
    }
  }

#ifdef DEBUG
  kgm_log() << "\nActor: " << actor->getName();
#endif

  return actor;
}
