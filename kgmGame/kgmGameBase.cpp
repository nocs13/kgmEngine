#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmEnvironment.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmBase/kgmIAudio.h"
#include "../kgmBase/kgmSound.h"
#include "../kgmBase/kgmBase.h"
#include "../kgmBase/kgmLog.h"

#include "../kgmSystem/kgmSystem.h"
#include "../kgmSystem/kgmThread.h"
#include "../kgmSystem/kgmOGL.h"
#include "../kgmSystem/kgmOAL.h"

#include "kgmIGame.h"
#include "kgmGameBase.h"
#include "kgmGamePhysics.h"
#include "kgmGameResources.h"

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
  :kgmOGLWindow(0, "kgmGameWindow", 0, 0, 640, 480, 24, false){
  m_game = this;
  kgmString sdata;
  kgmString spath;
  
  m_resources = 0;
  m_graphics = 0;
  m_physics = 0;
  m_system = 0;
  m_audio = 0;
  
  prev_width = 640;
  prev_height = 480;

  log("open settings...");
  m_settings = new kgmGameSettings();
  spath = m_settings->get("Path");

  log("open system...");
  m_system = new kgmSystem();

  log("check desktop dimensions...");
  m_system->getDesktopDimension(m_width, m_height);

  log("size window...");
  //setRect(0, 0, m_width,	m_height);
  
  log("open resources");
  m_resources = new kgmGameResources();
  m_resources->addPath(spath);
  
  log("open graphics...");
  m_graphics = getGC();
  if(!m_graphics)
    return;
  
  log("open physics...");
  m_physics = new kgmGamePhysics();
  
  log("open audio...");
  //m_audio	= new kgmOAL();
  
  log("open renderer...");
  m_render = new kgmGameGraphics(m_graphics, m_resources);
  m_render->resize(m_width, m_height);
  
  //  log("open data...");
  //  kgmString s;
  //  int       i = 0;
  
  //  m_paths.seek(0);
  //  while(!m_paths.eof()){
  //    kgmString s;
  //    s.alloc(128);
  //    u32 len = m_paths.reads(s.data(), 127, "\n", 1);
  //    if(len < 1)
  //      break;
  //    kgmString st(s.data(), len);
  //    m_resources->addPath(st);
  //    m_paths.seek(m_paths.position() + 1);
  //  }
  //  m_paths.seek(0);

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
  m_keymap[KEY_ENTER]  = (char)gbtn_start;

  m_keymap[KEY_X] = (char)gbtn_x;
  m_keymap[KEY_Z] = (char)gbtn_z;
}

kgmGameBase::kgmGameBase(kgmString &conf)
  :kgmOGLWindow(0, "kgmGameWindow", 0, 0, 640, 480, 24, false)
{
}


kgmGameBase::~kgmGameBase(){
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
  //if(m_audio)
  //  m_audio->release();

  log("free system...");
  if(m_system)
    m_system->release();

  m_eventListeners.clear();
}

kgmIGraphics* kgmGameBase::getGraphics(){
  return m_game->m_graphics;
}

kgmIPhysics* kgmGameBase::getPhysics(){
  return m_game->m_physics;
}

kgmISpawner* kgmGameBase::getSpawner(){
  return m_game->m_spawner;
}

kgmIAudio*  kgmGameBase::getAudio(){
  return m_game->m_audio;
}

kgmIVideo*  kgmGameBase::getVideo(){
  return m_game->m_video;
}

kgmIResources* kgmGameBase::getResources(){
  return m_game->m_resources;
}

kgmSystem*  kgmGameBase::getSystem(){
  return m_game->m_system;
}

void kgmGameBase::log(const char* msg){
  kgmLog::log(msg);
}

/*void kgmGameBase::settings(bool save){
 FILE* file;
 if(save){
  //Saving...
  file = fopen(set_file, "w");
  if(!file)
   return;
  fprintf(file, "Width\t\t\t%i\n", m_width);
  fprintf(file, "Height\t\t\t%i\n", m_height);
  fprintf(file, "Bpp\t\t\t%i\n", m_bpp);
  fprintf(file, "Fullscreen\t\t\t%i\n", m_fullscreen);
 }else{
  //Reading...
  file = fopen(set_file, "r");
  if(!file)
   return;
  char* buf = new char[128];
  while(!feof(file) || !ferror(file)){
   memset(buf, 0, sizeof(char) * 128);
   if(fscanf(file, "%s", buf) < 1)
    break;
   fprintf(stderr, buf);
   if(!strcmp(buf, "Window")){
    fscanf(file, "%i %i %i", &m_width, &m_height, &m_bpp);
    setRect(0, 0, m_width, m_height);
    m_render->resize(m_width, m_height);
   }else if(!strcmp(buf, "Fullscreen")){
    fscanf(file, "%i", &m_fullscreen);
    this->fullscreen((bool)m_fullscreen);
   }else if(!strcmp(buf, "BaseFont")){
    char* sbuf = new char[128];
    int r = 0, c = 0;
    memset(sbuf, 0, 128);
    fscanf(file, "%s %i %i ", sbuf, &r, &c);
    kgmString path(sbuf, strlen(sbuf));
    log("open font...");
    m_font = m_resources->getFont((char*)sbuf, r, c);
    m_render->setDefaultFont(m_font);
    if(!m_font)
     log("can't load font");
    delete [] sbuf;
   }else if(!strcmp(buf, "Data")){
    char* sbuf = new char[128];
    memset(sbuf, 0, 128);
    fscanf(file, "%s", sbuf);
    kgmString path(sbuf, strlen(sbuf));
    m_resources->addPath(path);
    delete [] sbuf;
   }
  }
  delete [] buf;
 }
}
*/

//
void kgmGameBase::onIdle(){
  /*
 static int tick = kgmTime::getTicks();
 static int frames = 0;
 static int fps = 0;
 static char buf[128];

 if(kgmTime::getTicks() - tick > 1000){
  fps = (frames > 0)?(frames):(1);
  frames = 0;
  getEnvironment()->time_per_frame = 1.0f / fps;
  sprintf(buf, "FPS: %i TPF: %f\0", fps,
          getEnvironment()->time_per_frame);
  tick =  kgmTime::getTicks();
 }else{
   frames++;
 }
//*/

  /* static float alpha = 0.0;
 static float m_time[4];
 vec3 v[2];
 mtx4 m;
 vec4 myvar;
 int i = 0;

  mtx4 mvw, mpr;
  int  rc[4];
  kgmIGraphics* gc = m_graphics;

  getRect(rc[0], rc[1], rc[2], rc[3]);
  gc->gcSetViewport(0, 0, rc[2], rc[3], 1.0, 1000.0);
  gc->gcClear(gcflag_color | gcflag_depth, 0xff0000ff, 1, 0);

  mpr.perspective(0.25 * PI, (float)rc[2]/(float)rc[3], 0.1, 10000000.0);
  mvw.lookat(vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1));
  //gc->gcSetMatrix(gcmtx_view, mvw.m);
  //gc->gcSetMatrix(gcmtx_proj, mpr.m);
  gc->gcSetMatrix(gcmtx_view, g_cam.mView.m);
  gc->gcSetMatrix(gcmtx_proj, g_cam.mProj.m);

  //  mvw.identity();
  //  mpr.identity();
  //  gc->gcSetMatrix(gcmtx_view, mvw.m);
  //  gc->gcSetMatrix(gcmtx_proj, mpr.m);

  gc->gcBegin();
  gcDrawGridlines(gc, 10, 20, 0x55555555);
  //gcDrawPlane(gc, vec3(0, 0, -1), 10, 10, 0xFFFF00FF);
  gc->gcRender();
  return;*/
  //if(fps > 60) return;
  switch(m_state){
  case State_None:
    break;
  case State_Load:
    break;
  case State_Pause:
    break;
  case State_Play:
    for(int i = 0; i < m_sensors.size(); i++)
      m_sensors[i]->sense();

    for(int i = 0; i < m_idles.size(); i++)
      m_idles[i]->idle();
    break;
  default:
    break;
  }

  if(m_render)
    m_render->render();
}

void kgmGameBase::onPaint(kgmIGraphics* gc){
}

void kgmGameBase::onKeyUp(int k){
  m_input[m_keymap[k]] = 0;
  onInputAction(m_keymap[k], 0);
}

void kgmGameBase::onKeyDown(int k){
  m_input[m_keymap[k]] = 1;

  if(k == KEY_UP)
    g_cam.move(0.1);
  else if(k == KEY_DOWN)
    g_cam.move(-0.1);
  else if(k == KEY_LEFT)
    g_cam.rotate(-0.01, 0);
  else if(k == KEY_RIGHT)
    g_cam.rotate(0.01, 0);

  onInputAction(m_keymap[k], 1);
}

void kgmGameBase::onMsMove(int k, int x, int y){
  static int sx = 0, sy = 0;

  g_cam.rotate(0.01 * (x - sx), 0.01 * (y - sy));
  m_input[grot_x] = x - sx; sx = x;
  m_input[grot_y] = y - sy; sy = y;
}

void kgmGameBase::onResize(int w, int h){
  kgmWindow::onResize(w, h);
  if(m_render)
    m_render->resize(w, h);

  float sw = (float)w / (float)prev_width;
  float sh = (float)h / (float)prev_height;
  for(int i = 0; i < m_guis.size(); i++)
    m_guis[i]->scale(sw, sh);
  //m_guis[i]->resize(w, h);
}

void kgmGameBase::onEvent(kgmEvent::Event* e){
  kgmWindow::onEvent(e);
  for(int i = 0; i < m_guis.size(); i++)
    m_guis[i]->onEvent(e);
}

void kgmGameBase::onInputAction(int action, int argument){
  for(int i = 0; i < m_eventListeners.length(); i++){
    if(m_eventListeners[i]->getMask() & InputEventListener::EventInput){
      m_eventListeners[i]->eventType = InputEventListener::EventInput;
      m_eventListeners[i]->eventAction = action;
      m_eventListeners[i]->eventArgument = argument;
      m_eventListeners[i]->onEvent();
    }
  }
}

//Game Functions
int kgmGameBase::gLoad(kgmString s)
{
  if(m_objects.size())
    gUnload();

  kgm_log() << kgm_log_label() << " " << "Loading game map " << (char*)s << "..." << "\n";
  m_state = State_Load;
  

  loadXml_II(s);

  if(m_objects.size()){
    //m_node->add(new kgmGameNode(new kgmCamera()));
    //m_render->add(m_node);
  }

  m_render->build();
  m_physics->build();

  m_state = State_Play;

  return m_state;
}

int kgmGameBase::gUnload()
{
  u32 state = m_state;
  state = State_Stop;
  
  if(m_render)
    m_render->clean();

  if(m_physics)
    m_physics->clear();

  if(m_logic)
      m_logic->clear();

  if(m_objects.length() > 0){
    for(int i = 0; i < m_objects.length(); i++)
      m_objects[i]->release();
    m_objects.clear();
  }

  m_state = state;

  return 0;
}

int kgmGameBase::gCommand(kgmString s)
{
  if(s == "exit")
    gQuit();

  if(s == "pause")
    m_state = State_Pause;

  if(s == "resume")
    m_state = State_Play;

  return true;
}

int kgmGameBase::gQuit(){
  m_state = State_Quit;
  m_quit = true;
  m_msAbs = true;
  m_loop = false;
  m_state = State_None;
  return 1;
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

void kgmGameBase::gLogic(){
  static u32 s_time = kgmTime::getTime();
  u32   d_time = kgmTime::getTime() - s_time;
  u32	c_time = 30;

  if(m_state != State_Play){
    return;
  }

  //m_input[grot_x] = 0;
  //m_input[grot_y] = 0;
  //m_input[grot_z] = 0;

  if(m_quit){
    gQuit();
    return;
  }

  if(d_time > c_time){
    s_time = kgmTime::getTime();
    m_physics->update(d_time / 1000.0);
  }
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
// Load XML and generate kgmGameNode object
//

kgmGameNode* kgmGameBase::loadXml(kgmString& path)
{
#define None     0
#define Material 1
#define Camera   2
#define Light    3
#define Actor    5
  
#define AttrString(node, id, val)		\
  {						\
  kgmString sid;				\
  sid = id;					\
  node->attribute(sid, val);			\
}
  
  kgmMemory<char> mem;
  if(!kgmIGame::getGame()->getResources()->getFile(path, mem)){
    kgmLog::log(kgmString("Can't find map %s") + kgmString(path));
    return false;
  }
  
  kgmXml xml(mem);
  if(!xml.m_node){
    kgmLog::log(kgmString("Not valid xml ") + kgmString(path));
    return false;
  }
  
  u32            type = 0;
  kgmMesh*        msh = 0;
  kgmCamera*      cam = 0;
  kgmLight*       lgt = 0;
  kgmMaterial*    mtl = 0;
  
  kgmList<kgmNode*> nodes;
  
  u32            vts = 0, fcs = 0;
  
  kgmObject* obj = 0;
  kgmActor* act = 0;

  for(int i = 0; i < xml.m_node->nodes(); i++){
    kgmString id, t;
    kgmXml::Node* node = xml.m_node->node(i);
    node->id(id);
    if(id == "kgmMaterial"){
      kgmString id;
      xmlAttr(node, "name", id);
      obj = mtl = new kgmMaterial();
      ((kgmMaterial*)obj)->setId(id);
      m_objects.add(obj);
    }else if(id == "kgmCamera"){
      obj = cam = new kgmCamera();
      m_objects.add(obj);
    }else if(id == "kgmLight"){
      obj = lgt = new kgmLight();
        m_objects.add(obj);
    }else if(id == "kgmMesh"){
      obj = msh = new kgmMesh();
        m_objects.add(obj);
    }else if(id == "kgmActor"){
      kgmString s;
      xmlAttr(node, "type", s);
      obj = act = new kgmActor();
      m_objects.add(obj);
    }
    
    for(int j = 0; j < node->nodes(); j++){
      kgmString id;
      kgmXml::Node* cnode = node->node(j);
      cnode->id(id);
      
      if(id == "Color"){
      }else if(id == "Ambient"){
      }else if(id == "Specular"){
      }else if(id == "Shininess"){
      }else if(id == "Transparency"){
      }else if(id == "Texture"){
      }else if(id == "Shader"){
      }else if(id == "Material"){
        kgmString data;
        xmlAttr(cnode, "name", data);
        kgmMaterial* mtl = 0;
        for(int i = 0; i < m_objects.size(); i++){
          if(m_objects[i]->isClass(kgmMaterial::Class) && ((kgmMaterial*)m_objects[i])->m_id == data){
            mtl = (kgmMaterial*)m_objects[i];
            break;
          }
        }
      }else if(id == "Vertices"){
        int len = 0, n = 0;
        kgmString data;

        xmlAttr(cnode, "length", len);
        kgmMesh::Vertex_P_N_C_T* v = (kgmMesh::Vertex_P_N_C_T*)msh->vAlloc(len, kgmMesh::FVF_P_N_C_T);
        cnode->data(data);
        char* pdata = data.data();
        for(int i = 0; i < len; i++){
          sscanf(pdata, "%f %f %f %f %f %f %f %f%n",
                 &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
                 &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
                 &v[i].uv.x, &v[i].uv.y, &n);
          v[i].col = 0xffffffff;
          (pdata) += (u32)n;
        }
      }else if(id == "Faces"){
        int len = 0, n = 0;
        kgmString data;

        xmlAttr(cnode, "length", len);
        kgmMesh::Face_16* f = (kgmMesh::Face_16*)msh->fAlloc(len, kgmMesh::FFF_16);
        cnode->data(data);
        char* pdata = data.data();
        for(int i = 0; i < len; i++){
          u32 fs[4];
          sscanf(pdata, "%i %i %i %n", &fs[0], &fs[1], &fs[2], &n);
          f[i].f[0] = fs[0];
          f[i].f[1] = fs[1];
          f[i].f[2] = fs[2];
          (pdata) += (u32)n;
        }
      }else if(id == "Position"){
        vec3 v;
        xmlAttr(cnode, "value", v);
        if(act)
            act->setPosition(v);
      }else if(id == "Rotation"){
        vec3 v;
        xmlAttr(cnode, "value", v);
        if(act)
          act->setRotation(v);
      }else if(id == "Quaternion"){
        quat q;
        xmlAttr(cnode, "value", q);
        if(act)
            act->setRotation(q);
      }
    }
  }

  return null; //m_objects.size();
}

//
//
//

//
// Load XML and generate kgmGameBase::Scene object
//

bool kgmGameBase::loadXml_II(kgmString& path)
{
#define TypeNone     0
#define TypeMaterial 1
#define TypeCamera   2
#define TypeLight    3
#define TypeMesh     4
#define TypeActor    5

#define AttrString(node, id, val)		\
  {						\
  kgmString sid;				\
  sid = id;					\
  node->attribute(sid, val);			\
}

  kgmMemory<char> mem;
  if(!kgmIGame::getGame()->getResources()->getFile(path, mem)){
    kgm_log() << "\nCan't find map " << (char*)path;
    return false;
  }

  kgmXml xml(mem);
  if(!xml.m_node){
    kgm_log() << "\nNot valid xml %s" << (char*)path;
    kgm_log() << "Error: " << "Not valid xml " << (char*)path << "\n";
    return false;
  }

  u32            type = TypeNone;
  kgmMesh*        msh = 0;
  kgmCamera*      cam = 0;
  kgmLight*       lgt = 0;
  kgmMaterial*    mtl = 0;
  kgmActor*       act = 0;
  kgmObject*      obj = 0;

  u32            vts = 0, fcs = 0;

  for(int i = 0; i < xml.m_node->nodes(); i++){
    kgmString id, t;
    kgmXml::Node* node = xml.m_node->node(i);
    node->id(id);

    kgm_log() << "Node: " << (char*)node->m_name << "\n";

    if(id == "kgmMaterial"){
      kgmString id;
      xmlAttr(node, "name", id);
      type = TypeMaterial;
      obj = mtl = new kgmMaterial();
      mtl->setId(id);
      m_render->add(mtl);
      m_objects.add(obj);
    }else if(id == "kgmCamera"){
      type = TypeCamera;
      //obj = cam = (kgmCamera*)act->camera;
    }else if(id == "kgmLight"){
      type = TypeLight;
      obj = lgt = new kgmLight();
      m_render->add(lgt);
      m_objects.add(obj);
    }else if(id == "kgmMesh"){
      type = TypeMesh;
      obj = msh = new kgmMesh();
      m_render->add(msh, 0);
    }else if(id == "kgmActor"){
      type = TypeActor;
      kgmString s;
      xmlAttr(node, "id", s);
      obj = act = gSpawn(s);
      if(act){
        m_render->add(act);
        m_objects.add(obj);
      }
    }

    for(int j = 0; j < node->nodes(); j++){
      kgmString id;
      kgmXml::Node* cnode = node->node(j);
      cnode->id(id);

      if(id == "Color"){
      }else if(id == "Ambient"){
      }else if(id == "Specular"){
      }else if(id == "Shininess"){
      }else if(id == "Transparency"){
      }else if(id == "Texture"){
      }else if(id == "Shader"){
      }else if(id == "Material"){
      }else if(id == "Vertices"){
        int len = 0, n = 0;
        kgmString data;

        xmlAttr(cnode, "length", len);
        kgmMesh::Vertex_P_N_C_T* v = (kgmMesh::Vertex_P_N_C_T*)msh->vAlloc(len, kgmMesh::FVF_P_N_C_T);
        cnode->data(data);
        char* pdata = data.data();
        for(int i = 0; i < len; i++){
          sscanf(pdata, "%f %f %f %f %f %f %f %f%n",
                 &v[i].pos.x, &v[i].pos.y, &v[i].pos.z,
                 &v[i].nor.x, &v[i].nor.y, &v[i].nor.z,
                 &v[i].uv.x, &v[i].uv.y, &n);
          v[i].col = 0xffffffff;
          (pdata) += (u32)n;
        }
      }else if(id == "Faces"){
        int len = 0, n = 0;
        kgmString data;

        xmlAttr(cnode, "length", len);
        kgmMesh::Face_16* f = (kgmMesh::Face_16*)msh->fAlloc(len, kgmMesh::FFF_16);
        cnode->data(data);
        char* pdata = data.data();
        for(int i = 0; i < len; i++){
          u32 fs[4];
          sscanf(pdata, "%i %i %i %n", &fs[0], &fs[1], &fs[2], &n);
          f[i].f[0] = fs[0];
          f[i].f[1] = fs[1];
          f[i].f[2] = fs[2];
          (pdata) += (u32)n;
        }
      }else if(id == "Position"){
        vec3 v;
        xmlAttr(cnode, "value", v);

        switch(type){
        case TypeCamera:
          m_render->camera().mPos = v;
          break;
        default:
          if(act)
            act->setPosition(v);
        }
      }else if(id == "Rotation"){
        vec3 v;
        xmlAttr(cnode, "value", v);
        if(act)
          act->setRotation(v);
      }else if(id == "Quaternion"){
        quat q;
        xmlAttr(cnode, "value", q);
        if(act)
          act->setRotation(q);
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
  kgmMemory<char> mem;

  kgm_log() << "\nSpawning Actor: " << a.data();

  if(!m_resources->getFile(a, mem))
    return 0;

  kgmXml xml(mem);
  if(!xml.m_node)
    return 0;

  actor = new kgmActor();
  for(int i = 0; i < xml.m_node->nodes(); i++){
    kgmString id, val;
    xml.m_node->node(i)->id(id);

    if(id == "Section"){
      actor->addSection();
      u32 cpart = actor->sections() - 1;

      for(int j = 0; j < xml.m_node->node(i)->nodes(); j++){
        xml.m_node->node(i)->node(j)->id(id);
        if(id == "Material"){
          xml.m_node->node(i)->attribute("value", val);
          actor->set(cpart, m_resources->getMaterial(val));
          kgm_log() << "\nMaterial: " << val.data();
        }else if(id == "Mesh"){
          xml.m_node->node(i)->attribute("value", val);
          actor->add(0, m_resources->getMesh(val));
        }else if(id == "Animation"){
          xml.m_node->node(i)->attribute("value", val);
          actor->set(0, m_resources->getAnimation(val));
        }else if(id == "Skeleton"){
          xml.m_node->node(i)->attribute("value", val);
          actor->set(0, m_resources->getSkeleton(val));
        }else if(id == "Dummy"){
          xml.m_node->node(i)->attribute("value", val);
        }
      }
    }
  }

  kgm_log() << "\nActor: " << actor->m_name.data();

  return actor;
}

/*
void kgmGameBase::gRender(kgmIGraphics* gc){
 static float alpha = 0.0;
 static float m_time[4];
 vec3 v[2];
 mtx4 m;
 vec4 myvar;
 int i = 0;

  mtx4 mvw, mpr;
  int  rc[4];

  getRect(rc[0], rc[1], rc[2], rc[3]);
  gc->gcSetViewport(0, 0, rc[2], rc[3], 1.0, 1000.0);
  gc->gcClear(gcflag_color | gcflag_depth, 0xff0000ff, 1, 0);

  mpr.perspective(0.25 * PI, (float)rc[2]/(float)rc[3], 0.1, 10000000.0);
  mvw.lookat(vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1));
  gc->gcSetMatrix(gcmtx_view, mvw.m);
  gc->gcSetMatrix(gcmtx_proj, mpr.m);

  if(m_map.camera){
    gc->gcSetMatrix(gcmtx_proj, m_map.camera->mProj.m);
    gc->gcSetMatrix(gcmtx_view, m_map.camera->mView.m);
  }else{
    mvw.identity();
    mpr.identity();
    gc->gcSetMatrix(gcmtx_view, mvw.m);
    gc->gcSetMatrix(gcmtx_proj, mpr.m);
  }

  gc->gcBegin();

  gcDrawGridlines(gc, 10, 20, 0x55555555);
  //gcDrawPlane(gc, vec3(0, 0, -1), 10, 10, 0xFFFF00FF);
  if(m_state == State_Play)
    m_render.render();

  //For last step draw gui
  gc->gc2DMode();
  for(int i = 0; i < m_guis.size(); i++)
    m_guis[i]->repaint(gc);
  gc->gc3DMode();

  gc->gcRender();
}
*/
