#include "../kgmMain/kgmObject.h"
#include "../kgmMain/kgmEnvironment.h"
#include "../kgmMain/kgmXml.h"
#include "../kgmMain/kgmFile.h"
#include "../kgmMain/kgmTime.h"
#include "../kgmMain/kgmIAudio.h"
#include "../kgmMain/kgmSound.h"

#include "../kgmSystem/kgmSystem.h"
#include "../kgmSystem/kgmThread.h"
#include "../kgmSystem/kgmOGL.h"
#include "../kgmSystem/kgmOAL.h"


#include "kgmIGame.h"
#include "kgmGameBase.h"
#include "kgmGamePhysics.h"
#include "kgmGameResources.h"

#include "kgmActor.h"

#include "../kgmGraphics/kgmDraw.inl"
#include "../kgmGraphics/kgmGuiTab.h"
/////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stack>


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

uint cmd_input = 0;
std::stack<uint> stack_input;

int   cc_value = 0;
float m_camDistance = 10.0f;

bool m_quit = false;

kgmCString c_map;


////////////////////////////////////
//                kgmGameBase
kgmGameBase::kgmGameBase()
:kgmOGLWindow(0, "kgmGameWindow", 0, 0, 640, 480, 24, false){
 m_game = this;
 kgmString sdata;
 
 m_resources = 0;
 m_graphics = 0;
 m_physics = 0;
 m_system = 0; 
 m_audio = 0;

 log("open system...");
  m_system = new kgmSystem();

 log("open settings...");
 settings();

 log("check desktop dimensions...");
 m_system->getDesktopDimension(m_width, m_height);

 log("size window...");
 //setRect(0, 0, m_width,	m_height);

 log("open resources");
 m_resources	= new kgmGameResources();

 log("open graphics...");
 m_graphics = getGC();
 if(!m_graphics)
	 return;

 m_render = new kgmGraphics(this);
 m_render->resize(m_width, m_height);

 log("open physics...");
 m_physics = new kgmGamePhysics();

 log("open audio...");
 m_audio	= new kgmOAL();

 log("open data...");
 kgmString s;
 int       i = 0;

 m_paths.seek(0);
 while(!m_paths.eof()){
  kgmString s;
  s.alloc(128);
  uint len = m_paths.reads(s.data(), 127, "\n", 1);
  if(len < 1)  break;
  kgmString st(s.data(), len);
  m_resources->addPath(st);
  m_paths.seek(m_paths.position() + 1);
 }
 m_paths.seek(0);

 log("open font...");
 m_font = m_resources->getFont((char*)"arial.tga", 16, 16);
 if(!m_font)
   log("can't load font");

 log("set input map...");
 for(int i = 0; i < sizeof(m_keymap); i++)
   m_keymap[i] = 0;
 for(int i = 0; i < sizeof(m_input); i++)
   m_input[i] = 0;

 m_keymap[KEY_LEFT]  = m_keymap[KEY_A] = (char)gbtn_left;
 m_keymap[KEY_RIGHT] = m_keymap[KEY_D] = (char)gbtn_right;
 m_keymap[KEY_UP]    = m_keymap[KEY_W] = (char)gbtn_up;
 m_keymap[KEY_DOWN]  = m_keymap[KEY_S] = (char)gbtn_down;

 log("create logic manager...");
 m_logic	= new kgmGameLogic();

}

kgmGameBase::~kgmGameBase(){
 log("free graphics renderer...");
 if(m_render) delete m_render;

 log("free scenes...");
 for(int i = 0; i < m_scenes.size(); i++)
   delete m_scenes[i];
 m_scenes.clear();

 log("free gui...");
 for(int i = 0; i < m_guis.size(); i++)
   delete m_guis[i];
 m_guis.clear();

 log("free game manager...");
 if(m_logic)        delete m_logic;

 log("free resources...");
 if(m_resources)    delete m_resources;

 log("free physics...");
 if(m_physics)      delete m_physics;

 log("free audio...");
 if(m_audio)        delete m_audio;

 log("free system...");
 if(m_system)       delete m_system;
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
 FILE *o = fopen(log_file, "a");
 if(!o || !msg)
  return;
 char *m = new char[1024];
 memset(m, 0, 1024);
 sprintf(m, "%s\n\0", msg);
 fprintf(o, m);
 fclose(o);
 delete [] m;
 printf("\nlog: %s", msg);
}

void kgmGameBase::settings(bool save){
 FILE* file;
 if(save){
  file = fopen(set_file, "w");
  if(!file)
   return;
  fprintf(file, "Width\t\t\t%i\n", m_width);
  fprintf(file, "Height\t\t\t%i\n", m_height);
  fprintf(file, "Bpp\t\t\t%i\n", m_bpp);
  fprintf(file, "Fullscreen\t\t\t%i\n", m_fullscreen);
 }else{
  file = fopen(set_file, "r");
  if(!file)
   return;
  char* buf = new char[128];
  while(!feof(file) || !ferror(file)){
   memset(buf, 0, sizeof(char) * 128);
   if(fscanf(file, "%s", buf) < 1)
    break;
   if(!strcmp(buf, "Width")){
    fscanf(file, "%i", &m_width);
   }else if(!strcmp(buf, "Height")){
    fscanf(file, "%i", &m_height);
   }else if(!strcmp(buf, "Bpp")){
    fscanf(file, "%i", &m_bpp);
   }else if(!strcmp(buf, "Fullscreen")){
    fscanf(file, "%i", &m_fullscreen);
   }else if(!strcmp(buf, "Data")){
    char* sbuf = new char[128]; 
    memset(sbuf, 0, 128);
    fscanf(file, "%s", sbuf);
    kgmString path(sbuf, strlen(sbuf));
    m_paths += path;
    kgmString eol("\n", 1);
    m_paths += eol;
    delete [] sbuf;
   }
  }
  delete [] buf;
 }
}

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

 //if(fps > 60) return;
  switch(m_state){
  case State_None:
    break;
  case State_Load:
    break;
  case State_Pause:
    break;
  case State_Play:
    gLogic();
    if(m_render)
      m_render->render();
    break;
  }
}

void kgmGameBase::onPaint(kgmIGraphics* gc){
}

void kgmGameBase::onKeyUp(int k){
 m_input[m_keymap[k]] = 0;
}

void kgmGameBase::onKeyDown(int k){
 m_input[m_keymap[k]] = 1;
}

void kgmGameBase::onMsMove(int k, int x, int y){
  static int sx = 0, sy = 0;

  m_input[grot_x] = x - sx; sx = x;
  m_input[grot_y] = y - sy; sy = y;
}

void kgmGameBase::onResize(int w, int h){
 static int pw = w, ph = h;
 float sw = (float)w / (float)pw,
       sh = (float)h / (float)ph;
 pw = w, ph = h;
 kgmWindow::onResize(w, h);
 if(m_render)
   m_render->resize(w, h);
 for(int i = 0; i < m_guis.size(); i++)
   m_guis[i]->scale(sw, sh);
}

void kgmGameBase::onEvent(kgmEvent::Event* e){
 kgmWindow::onEvent(e);
 for(int i = 0; i < m_guis.size(); i++) 
   m_guis[i]->onEvent(e);
}

//Game Functions
int kgmGameBase::gLoad(kgmString s)
{
  uint state = m_state;
  if(state == State_Load)
    return 0;

  fprintf(stderr, "\nClear map...");
  m_state = State_Stop;

  if(m_logic)
    m_logic->clear();

  fprintf(stderr, "\nLoading game map %s...", (char*)s);
  m_state = State_Load;
  
  for(int i = 0; i < m_scenes.size(); i++){
    m_render->erase(m_scenes[i]->node);
    delete m_scenes[i];
  }
  m_scenes.clear();

  Scene* scene = loadXml(s);
  if(scene){
    m_render->add(scene->node);
    m_scenes.add(scene);
  }

  m_state = State_Play;
  return m_state;
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

uint kgmGameBase::gState(){
 return m_state;
}

bool kgmGameBase::gResult(){
 return m_result;
}


void kgmGameBase::gLogic(){
 static uint s_time = kgmTime::getTime();
 uint   d_time = kgmTime::getTime() - s_time;
 uint	c_time = 30;

 if(m_state != State_Play){
  return;
 }

 if(m_logic)
   m_logic->sense(kgmGameLogic::SensorInput, m_input);
 m_input[grot_x] = 0;
 m_input[grot_y] = 0;
 m_input[grot_z] = 0;

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

/////////////////////////
kgmGameBase::Scene* kgmGameBase::loadXml(kgmString& path)
{
#define None     0
#define Material 1
#define Camera   2
#define Light    3
#define Actor    5

#define AttrString(node, id, val)\
{\
  kgmString sid;\
  sid = id;\
  node->attribute(sid, val);\
}

 kgmMemory<char> mem;
 if(!kgmIGame::getGame()->getResources()->getFile(path, mem)){
   fprintf(stderr, "\nCan't find map %s", (char*)path);
   return false;
 }

 kgmXml xml(mem);
 if(!xml.m_node){
   fprintf(stderr, "\nNot valid xml %s", (char*)path);
   return false;
 }

 uint            type = 0;
 kgmMesh*        msh = 0;
 kgmCamera*      cam = 0;
 kgmLight*       lgt = 0;
 kgmActor*       act = 0;
 kgmMaterial*    mtl = 0;

 kgmList<kgmNode*> nodes;

 uint            vts = 0,
                 fcs = 0;

 Scene* scn = new Scene;

 for(int i = 0; i < xml.m_node->nodes(); i++){
  kgmString id, t;
  kgmXml::Node* node = xml.m_node->node(i);
  node->id(id);
  if(id == "kgmMaterial"){
    kgmString id;
    xmlAttr(node, "name", id);
    mtl = new kgmMaterial();
    mtl->setId(id);
    scn->node->add(new kgmNode(mtl));
    scn->objects.add(mtl);
  }else if(id == "kgmCamera"){
   cam = new kgmCamera();
   type = Camera;
   scn->node->add(new kgmNode(cam));
   scn->objects.add(cam);
  }else if(id == "kgmLight"){
   lgt = new kgmLight();
   type = Light;
   xmlAttr(node, "name", lgt->m_id);
   scn->node->add(new kgmNode(lgt));
   scn->objects.add(lgt);
  }else if(id == "kgmMesh"){
    msh = new kgmMesh();
    scn->node->add(new kgmNode(msh));
    scn->objects.add(msh);
  }else if(id == "kgmActor"){
   kgmString s;
   xmlAttr(node, "type", s);
   act = 0;
   act = kgmIGame::getGame()->gSpawn(s);
   if(act){
     type = Actor;
     xmlAttr(node, "name", act->m_name);
     scn->actors.add(act);
     kgmNode* n = new kgmNode();
     scn->node->add(n);
     n->add(new kgmNode(&act->m_transform));
     for(int i = 0; i < act->m_sections.size(); i++){
       kgmNode* ns = new kgmNode();
       n->add(ns);
       ns->add(new kgmNode(act->m_sections[i].animation));
       ns->add(new kgmNode(act->m_sections[i].skeleton));
       for(int j = 0; j < act->m_sections[i].meshes.size(); j++)
         ns->add(new kgmNode(act->m_sections[i].meshes[j]));
     }
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
    kgmString data;
    xmlAttr(cnode, "name", data);
    kgmMaterial* mtl = 0;
    for(int i = 0; i < scn->objects.size(); i++){
      if(scn->objects[i]->instanceof(kgmMaterial::Class) && ((kgmMaterial*)scn->objects[i])->m_id == data){
        mtl = (kgmMaterial*)scn->objects[i];
        break;
      }
    }
    kgmNode* mn = scn->node->find(kgmNode::NodeMaterial, mtl);
    if(mn)
      mn->add(new kgmNode(msh));
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
     (pdata) += (uint)n;
    }
   }else if(id == "Faces"){
    int len = 0, n = 0;
    kgmString data;

    xmlAttr(cnode, "length", len);
    kgmMesh::Face_16* f = (kgmMesh::Face_16*)msh->fAlloc(len, kgmMesh::FFF_16);
    cnode->data(data);
    char* pdata = data.data();
    for(int i = 0; i < len; i++){
      uint fs[4];
      sscanf(pdata, "%i %i %i %n", &fs[0], &fs[1], &fs[2], &n);
      f[i].f[0] = fs[0];
      f[i].f[1] = fs[1];
      f[i].f[2] = fs[2];
      (pdata) += (uint)n;
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
 scn->node->print();
 return scn;
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

  fprintf(stderr, "\nSpawning Actor: %s", a.data());
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
      uint cpart = actor->sections() - 1;

      for(int j = 0; j < xml.m_node->node(i)->nodes(); j++){
        xml.m_node->node(i)->node(j)->id(id);
        if(id == "Material"){
          xml.m_node->node(i)->attribute("value", val);
          actor->set(cpart, m_resources->getMaterial(val));
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
  fprintf(stderr, "\nActor: %s", actor->m_name.data());
  return actor;
}
