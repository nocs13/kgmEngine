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
#include "kgmGameBase.h"
#include "kgmGamePhysics.h"
#include "kgmGameResources.h"

#include "kgmGameObject.h"
#include "kgmActor.h"

//#include "../kgmGraphics/kgmDraw.inl"
#include "../kgmGraphics/kgmGuiTab.h"
#include "../kgmGraphics/kgmGraphics.h"
/////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

kgmGameBase* kgmGameBase::m_game;
kgmIGame* kgmIGame::getGame(){
  return kgmGameBase::m_game;
}

#define		BWIDTH		640
#define		BHEIGHT		480

const char *log_file = "kgmLog.txt";
const char *set_file = "kgmEngine.conf";

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

kgmCamera g_cam;
////////////////////////////////////
//                kgmGameBase
kgmGameBase::kgmGameBase()
  :kgmOGLWindow(0, "kgmGameWindow", 0, 0, BWIDTH, BHEIGHT, 24, false){
  m_game = this;
  kgmString sdata;
  kgmString spath;

  m_resources = 0;
  m_graphics = 0;
  m_physics = 0;
  m_system = 0;
  m_audio = 0;

  prev_width  = BWIDTH;
  prev_height = BHEIGHT;

  log("open settings...");
  m_settings = new kgmGameSettings();
  spath = m_settings->get("Path");

  log("open system...");
  m_system = new kgmSystem();

  log("check desktop dimensions...");
  m_system->getDesktopDimension(m_width, m_height);

  //log("size window...");
  //setRect(0, 0, m_width,	m_height);

  initResources();

  log("open graphics...");
  m_graphics = getGC();
  if(!m_graphics)
    return;

  log("init physics...");
  initPhysics();

  log("open renderer...");
  m_render = new kgmGameGraphics(m_graphics, m_resources);
  m_render->resize(m_width, m_height);
  m_render->setGuiStyle(kgmGameTools::genGuiStyle(m_resources, "gui_style.kgm"));

  log("init game audio...");
  initAudio();
  log("init game logic...");
  initLogic();

  log("open font...");
  m_font = m_resources->getFont((char*)"arial.tga", 16, 16);

  if(!m_font)
    log("can't load font");
  else
    m_render->setDefaultFont(m_font );

  log("set input map...");
  for(int i = 0; i < sizeof(m_keymap); i++)
    m_keymap[i] = 0;
  for(int i = 0; i < sizeof(m_input); i++)
    m_input[i] = 0;

  m_keymap[KEY_LEFT]  = m_keymap[KEY_A] = (char)gbtn_left;
  m_keymap[KEY_RIGHT] = m_keymap[KEY_D] = (char)gbtn_right;
  m_keymap[KEY_UP]    = m_keymap[KEY_W] = (char)gbtn_up;
  m_keymap[KEY_DOWN]  = m_keymap[KEY_S] = (char)gbtn_down;
  m_keymap[KEY_ESCAPE]  = (char)gbtn_exit;
  m_keymap[KEY_ENTER]   = (char)gbtn_start;
  m_keymap[KEY_LSHIFT]  = (char)gbtn_n;
  m_keymap[KEY_RSHIFT]  = (char)gbtn_n;

  m_keymap[KEY_X] = (char)gbtn_x;
  m_keymap[KEY_Y] = (char)gbtn_y;
  m_keymap[KEY_Z] = (char)gbtn_z;

  m_gamemode = true;
  m_state    = State_None;
}

kgmGameBase::kgmGameBase(kgmString &conf)
  :kgmOGLWindow(0, "kgmGameWindow", 0, 0, 640, 480, 24, false)
{
}

kgmGameBase::~kgmGameBase(){
}

kgmIGraphics* kgmGameBase::getGraphics(){
  return m_game->m_graphics;
}

kgmIPhysics* kgmGameBase::getPhysics(){
  return m_game->m_physics;
}

kgmISpawner* kgmGameBase::getSpawner(){
  return null;
}

kgmIAudio*  kgmGameBase::getAudio(){
  return (kgmIAudio*)m_game->m_audio;
}

kgmIVideo*  kgmGameBase::getVideo(){
  return m_game->m_video;
}

kgmILogic*  kgmGameBase::getLogic(){
  return m_game->m_logic;
}

kgmIResources* kgmGameBase::getResources(){
  return m_game->m_resources;
}

kgmSystem*  kgmGameBase::getSystem(){
  return m_game->m_system;
}

kgmWindow*  kgmGameBase::getWindow(){
  return (kgmWindow*)this;
}

kgmEnvironment*  kgmGameBase::getEnvironment(){
  return m_settings;
}

void kgmGameBase::initResources()
{
  log("init resources");
  m_resources = new kgmGameResources();
  m_resources->addPath(m_settings->get("Path"));
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

}

void kgmGameBase::initAudio(){
//#ifdef WIN32
//#else
  m_audio = new kgmGameAudio();
//#endif
}

void kgmGameBase::initLogic(){
  m_logic = new kgmGameLogic();
}

void kgmGameBase::log(const char* msg){
  kgmLog::log(msg);
}
//

void kgmGameBase::onIdle(){

  static int tick = kgmTime::getTicks();
  static int frames = 0;
  static int fps = 1;
  static char buf[128] = {0};

  if(kgmTime::getTicks() - tick > 1000)
  {
    fps = frames;
    frames = 1;
    tick =  kgmTime::getTicks();
  }
  else
  {
    frames++;
  }

  // if(fps > 60)
  //  return;

  switch(m_state){
  case State_None:
    break;
  case State_Load:
    break;
  case State_Uload:
    gUnload();
    m_state = State_None;
    break;
  case State_Pause:
    break;
  case State_Play:
    if(m_logic)
      m_logic->update(1000 / fps);

    if(m_physics)
      m_physics->update(1000 / fps);
    break;
  case State_Stop:
    break;
  default:
    break;
  }

  if(m_render)
    m_render->render();

#ifdef TEST
  kgmObject::listObjects();
  //kgm_log() << "State is: " << (s32)m_state;
#endif
}

void kgmGameBase::onPaint(kgmIGraphics* gc){
}

void kgmGameBase::onClose()
{
  log("free graphics renderer...");
  if(m_render)
    m_render->release();

  log("free scenes...");

  log("free gui...");
  for(int i = 0; i < m_guis.size(); i++)
    m_guis[i]->release();
  m_guis.clear();

  log("free resources...");
  if(m_resources)
    m_resources->release();

  log("free physics...");
  if(m_physics)
    m_physics->release();

  log("free audio...");
  if(m_audio)
  {
    m_audio->release();
    //delete m_audio;
  }

  log("free system...");
  if(m_system)
    m_system->release();

  kgmOGLWindow::onClose();
}

void kgmGameBase::onKeyUp(int k){
  if(m_logic && m_state == State_Play && m_input[m_keymap[k]] != 0)
  {
    m_logic->input(m_keymap[k], 0);
    m_input[m_keymap[k]] = 0;
  }
}

void kgmGameBase::onKeyDown(int k){
  if(m_logic && (m_state == State_Play) && (m_input[m_keymap[k]] != 1))
  {
    m_logic->input(m_keymap[k], 1);
    m_input[m_keymap[k]] = 1;
  }
}

void kgmGameBase::onMsMove(int k, int x, int y){
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

void kgmGameBase::onResize(int w, int h){
  kgmWindow::onResize(w, h);
  if(m_render)
    m_render->resize(w, h);

  float sw = (float)w / (float)prev_width;
  float sh = (float)h / (float)prev_height;
  for(int i = 0; i < m_guis.size(); i++)
    m_guis[i]->scale(sw, sh);
}

void kgmGameBase::onEvent(kgmEvent::Event* e){
  kgmWindow::onEvent(e);

  for(int i = 0; i < m_guis.size(); i++)
  {
    m_guis[i]->onEvent(e);
  }
}

//Game Functions
int kgmGameBase::gLoad(kgmString s)
{
  gUnload();

  kgm_log() << kgm_log_label() << " " << "Loading game map " << (char*)s << "..." << "\n";
  m_state = State_Load;

  loadXml(s);

  if(m_logic)   m_logic->prepare();
  if(m_render)  m_render->build();
  if(m_physics) m_physics->build();

  m_state = State_Play;

  return m_state;
}

int kgmGameBase::gUnload()
{
  u32 state = m_state;
  state = State_Stop;

  kgm_log() << "\nUnloading...";

  if(m_logic)
    m_logic->clear();

  if(m_audio)
    m_audio->clear();

  if(m_physics)
    m_physics->clear();

  if(m_render)
    m_render->clear();

  m_state = State_None;
  kgm_log() << "\nUnloaded";

  return 0;
}

int kgmGameBase::gCommand(kgmString s)
{
  if(s == "exit")
    gQuit();
  else if(s == "pause")
    m_state = State_Pause;
  else if(s == "resume")
    m_state = State_Play;

  return true;
}

int kgmGameBase::gQuit(){
  m_state = State_Quit;
  m_quit = true;
  m_msAbs = true;
  m_state = State_None;
  close();
  return 1;
}

int kgmGameBase::gSwitch(u32 state)
{
  m_state = state;

  return m_state;
}

u32 kgmGameBase::gState(){
  return m_state;
}

void kgmGameBase::gPause(bool s){
  if(s && m_state == State_Play)
  {
    m_state = State_Pause;
  }

  if(!s && m_state == State_Pause)
  {
    m_state = State_Play;
  }
}

bool kgmGameBase::gAppend(kgmGameObject* go)
{
  if(!go || m_state == State_None || m_state == State_Quit)
    return false;

  if(m_render && go->getVisual())
    m_render->add(go->getVisual());

  if(m_physics && go->getBody())
    m_physics->add(go->getBody());

  if(m_logic)
    m_logic->add(go);

#ifdef TEST
  if(m_render && go->getBody())
    m_render->add(go->getBody());
#endif

  return true;
}

kgmGameObject* kgmGameBase::gObject(kgmString s)
{
  return null;
}

///////////////////////////////////////
kgmTexture* kgmGameBase::getTexture(char* id){
  return kgmIGame::getGame()->getResources()->getTexture(id);
}

kgmShader* kgmGameBase::getShader(char* id){
  return kgmIGame::getGame()->getResources()->getShader(id);
}

kgmMesh* kgmGameBase::getMesh(char* id){
  return kgmIGame::getGame()->getResources()->getMesh(id);
}

kgmAnimation* kgmGameBase::getAnimation(char* id){
  return kgmIGame::getGame()->getResources()->getAnimation(id);
}

kgmActor* kgmGameBase::getActor(kgmString id){
  return gSpawn(id);
}

/////////////////////////
//**************Load Map**********/////////
/////////////////
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
  double         a[3];
  sid = id;
  node->attribute(sid, v);
  sscanf(v.data(), "%f %f %f", &a[0], &a[1], &a[2]);
  val = vec3(a[0], a[1], a[2]);
}

inline void xmlAttr(kgmXml::Node* node, const char* id, quat& val)
{
  kgmString sid, v;
  double         a[4];
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

  kgmMemory<char> mem;
  if(!kgmIGame::getGame()->getResources()->getFile(path, mem)){
    kgm_log() << "\nCan't find map " << (char*)path;
    return false;
  }

  kgmXml xml;
  if(kgmXml::XML_ERROR == xml.open(mem)){
    kgm_log() << "\nNot valid xml %s" << (char*)path;
    kgm_log() << "Error: " << "Not valid xml " << (char*)path << "\n";

    return false;
  }

  u32             type = TypeNone;
  kgmMesh*        msh = 0;
  kgmCamera*      cam = 0;
  kgmLight*       lgt = 0;
  kgmMaterial*    mtl = 0;
  kgmActor*       act = 0;
  kgmGameObject*  gob = 0;
  kgmObject*      obj = 0;

  u32             vts = 0,
      fcs = 0;

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
      kgm_log() << "Node: " << (char*)id << "\n";

      if(id == "kgmMaterial")
      {
        kgmString id;
        xml.attribute("name", id);
        type = TypeMaterial;
        obj = mtl = new kgmMaterial();
        mtl->setId(id);
        m_render->add(mtl);
        mtl->release();
      }
      else if(id == "kgmCamera")
      {
        type = TypeCamera;
      }
      else if(id == "kgmLight")
      {
        type = TypeLight;
        obj = lgt = new kgmLight();
        m_render->add(lgt);
        lgt->release();
      }
      else if(id == "kgmMesh")
      {
        type = TypeMesh;
        obj = msh = new kgmMesh();
        m_render->add(msh, 0);
        msh->release();
      }
      else if(id == "kgmActor")
      {
        type = TypeActor;
        kgmString s, sgrp;
        xml.attribute("object", s);
        xml.attribute("group", sgrp);
        obj = act = gSpawn(s);

        if(act)
        {
          xml.attribute("name", s);
          act->setId(s);
          xml.attribute("player", s);

          if(s == "on")
          {
            act->m_gameplayer = true;

            if(m_gamemode)
              m_render->linkCamera(act->getVisual(), 10.0f, 10.0f);
          }

          if(sgrp.length() > 0)
            act->setGroup(kgmConvert::toInteger(sgrp));

          gAppend(act);
#ifdef TEST
          if(act && act->getBody())
            m_render->add(act->getBody());
#endif
          act->release();
        }
      }
      else if(id == "kgmGameObject")
      {
        type = TypeGameObject;
        kgmString s, sgrp;

        xml.attribute("object", s);
        xml.attribute("group", sgrp);
        obj = gob = gObject(s);

        if(sgrp.length() > 0)
          gob->setGroup(kgmConvert::toInteger(sgrp));

        gAppend(gob);
#ifdef TEST
        if(gob && gob->getBody())
          m_render->add(gob->getBody());
#endif
        gob->release();
      }
      else if(id == "Vertices")
      {
        int len = 0, n = 0;
        kgmString data;

        xml.attribute("length", value);
        sscanf(value.data(), "%i", &len);
        kgmMesh::Vertex_P_N_C_T* v = (kgmMesh::Vertex_P_N_C_T*)msh->vAlloc(len, kgmMesh::FVF_P_N_C_T);
        m_data = "vertices";
      }
      else if(id == "Faces")
      {
        int len = 0, n = 0;
        kgmString data;

        xml.attribute("length", value);
        sscanf(value.data(), "%i", &len);
        kgmMesh::Face_16* f = (kgmMesh::Face_16*)msh->fAlloc(len, kgmMesh::FFF_16);
        m_data = "faces";
      }
      else if(id == "Polygon")
      {
        kgmString data;

        xml.attribute("vertices", value);
        sscanf(value.data(), "%i", &vts);
        m_data = "polygon";
      }
    }
    else if(xstate == kgmXml::XML_TAG_CLOSE)
    {
      kgmString data;
      id = xml.m_tagName;
      kgm_log() << "Node: " << (char*)id << "\n";

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
          mtl->m_transparency = 1.0f - a;

          if(a < 1.0f)
          {
            mtl->m_alpha = true;
          }
        }
      }
      else if(id == "Cull")
      {
        mtl->m_2side = true;
      }
      else if(id == "map_color")
      {
        if(xml.attribute("value", data))
        {
          mtl->m_tex_color = m_resources->getTexture(data);
        }
      }
      else if(id == "map_normal")
      {
        kgmString data;

        if(xml.attribute("value", data))
        {
          mtl->m_tex_normal = m_resources->getTexture(data);
        }
      }
      else if(id == "map_specular")
      {
        kgmString data;

        if(xml.attribute("value", data))
        {
          mtl->m_tex_specular = m_resources->getTexture(data);
        }
      }
      else if(id == "Shader")
      {
        kgmString data;

        if(xml.attribute("value", data))
        {
          mtl->m_shader = (kgmMaterial::Shader)m_render->getShaderId(value);
        }
      }
      else if(id == "Material")
      {
        if(type == TypeMesh)
        {
          kgmString data;
          kgmMaterial* mtl = null;

          if(xml.attribute("name", data))
          {
            m_render->get(data, &mtl);
            m_render->set(msh, mtl);
          }
        }
      }
      else if(id == "Position")
      {
        vec3 v;
        xml.attribute("value", value);
        sscanf(value.data(), "%f %f %f", &v.x, &v.y, &v.z);

        switch(type){
        case TypeCamera:
          m_render->camera().mPos = v;
          break;
        case TypeLight:
          if(lgt)
            lgt->position = v;
          break;
        case TypeActor:
          if(act)
            act->setPosition(v);
          break;
        case TypeGameObject:
          if(gob)
            gob->setPosition(v);
          break;
        default:
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
          act->setRotation(v);
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
          act->setQuaternion(q);
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
        for(int i = 0; i < msh->vcount(); i++){
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
        for(int i = 0; i < msh->fcount(); i++){
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
        char* pdata = xml.m_tagData.data();
        m_data = "";
      }
      else if(m_data == "polygon")
      {
        int n = 0;
        char* pdata = xml.m_tagData.data();

        if(vts > 2)
        {
          vec3* v = new vec3[vts];

          for(int i = 0; i < vts; i++)
          {
            sscanf(pdata, "%f %f %f %n", &v[i].x, &v[i].y, &v[i].z, &n);
            (pdata) += (u32)n;
          }

          for(int i = 1; i < vts - 1; i++)
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

//kgmNode* kgmGameMap::loadBin(kgmString& path)
//{
//  return 0;
//}


bool kgmGameBase::gMapBinary(kgmString& path)
{
  return true;
}

/////////////////
kgmActor* kgmGameBase::gSpawn(kgmString a){
  kgmActor*       actor = 0;
  kgmString       type = a;
  kgmMemory<char> mem;

  kgm_log() << "\nSpawning Actor: " << a.data();

  if(!m_resources->getFile(a, mem))
  {
    if(!m_resources->getFile(a += ".act", mem))
    {
      if(!m_resources->getFile(a += ".kgm", mem))
      {
        return (kgmActor*)gObject(type);
      }
    }
  }

  kgmXml xml(mem);

  if(!xml.m_node)
    return 0;

  kgmXml::Node* a_node = null;

  for(int i = 0; i < xml.m_node->nodes(); i++){
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
  actor = (m_logic)?((kgmActor*)gObject(stype)):(new kgmActor());

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
      sscanf(val.data(), "%f", &actor->getBody()->m_mass);
    }
    else if(id == "Bound")
    {
      float a[3];
      a_node->node(i)->attribute("value", val);
      sscanf(val.data(), "%f%f%f", &a[0], &a[1], &a[2]);
      actor->getBody()->m_bound.min = vec3(-0.5 * a[0], -0.5 * a[1], 0.0);
      actor->getBody()->m_bound.max = vec3( 0.5 * a[0],  0.5 * a[1], a[2]);
    }
    else if(id == "Shapexxx")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "convex")
      {
        kgmMemory<s8> mem;
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

                    polygon3* pol = new polygon3(count);
                    vec3      v;
                    int       n = 0;
                    char*     pdata = node->m_data.data();

                    for(int k = 0; k < count; k++)
                    {
                      sscanf(pdata, "%f %f %f%n", &v.x, &v.y, &v.z, &n);
                      (pdata) += (u32)n;
                      pol->m_points[k] = v;
                    }

                    actor->getBody()->m_convex.add(pol);
                  }
                }
              }
            }
          }
        }

        if(actor->getBody()->m_convex.size() > 0)
          actor->getBody()->m_shape == kgmBody::ShapePolyhedron;
      }
    }
    else if(id == "Gravity")
    {
      a_node->node(i)->attribute("value", val);

      if(val == "true")
        actor->getBody()->m_gravity = true;
      else
        actor->getBody()->m_gravity = false;
    }
    else if(id == "Dummies")
    {
      kgmMemory<char> mem;
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
              dummy->release();
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

      kgmGameObject* go = (isactor) ? (gSpawn(type)) : (gObject(type));

      if(go)
      {
        go->setId(cid);
        go->setParent(actor);

        kgmDummy* dm = actor->dummy(dummy);

        if(dm)
          dm->attach(go, kgmDummy::AttachToObject);

        gAppend(go);
        go->release();
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
          mtl = m_resources->getMaterial(val);
          kgm_log() << "\nMaterial: " << val.data();
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
        actor->getVisual()->addVisual(msh, mtl);
        actor->getVisual()->setAnimation(anm);
        actor->getVisual()->setSkeleton(skl);

        msh->release();
        if(skl) skl->release();
        if(mtl) mtl->release();
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

      state->sound = null;
      state->animation = null;

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
          state->sound = m_resources->getSound(s);
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
        actor->m_options.add(id, val);
      }
    }
  }

  kgm_log() << "\nActor: " << actor->getId();

  return actor;
}

