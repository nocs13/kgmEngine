#include "kgmGameResources.h"
#include "kgmIGame.h"
#include "kgmGameTools.h"


#include "../kgmBase/kgmEnvironment.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmIAudio.h"
#include "../kgmBase/kgmXml.h"

#include "../kgmSystem/kgmSystem.h"

#include "kgmActor.h"

static char 	 str_buf[256];

kgmGameResources::kgmGameResources(){
}

kgmGameResources::~kgmGameResources(){
 release();
}

void kgmGameResources::release(){
 int i = 0;

 for(i = 0; i < m_paths.size(); i++)
  delete m_paths[i];
 m_paths.clear();

 for(i = 0; i < m_resources.size(); i++){
  if(m_resources[i] == 0)
   continue;
  m_resources[i]->release();
 }
 m_resources.clear();
}

void kgmGameResources::add(kgmResource* r){
 bool exist = false;
 if(!r) 
	 return;
 return;
 for(int i = 0; i < m_resources.size(); i++){
  if(r == m_resources[i]){
	  exist = true;
	  break;
  }
 }
 if(exist) return;
 m_resources.add(r);
}

void kgmGameResources::remove(kgmResource* r){
 if(!r) 
	 return;
 for(int i = 0; i < m_resources.size(); i++){
  if(r == m_resources[i]){
   r->release();
   if(r->references() <= 1)
	m_resources.erase(i);
   break;
  }
 }
}

void kgmGameResources::addPath(kgmString& s){
 Path* path = 0;
 if(kgmSystem::isDirectory(s)){
  path = new Path();
  path->type = 2;
  path->path = s;
 }else if(kgmSystem::isFile(s)){
  path = new Path();
  path->type = 1;
  path->path = s;
  path->archive.open(s);
 }

 if(path)
  m_paths.add(path);
}

bool kgmGameResources::getFile(char* id, kgmMemory<char>& m){
 kgmString path;
 bool  res = false;
 int   i = 0;

 fprintf(stderr, "\nLoad resource: %s", id);

#ifdef WIN32
 const kgmString delim((const char*)"\\", 1);
#else
 const kgmString delim((const char*)"/", 1);
#endif

 for(i = 0; i < m_paths.size(); i++){
  kgmFile file;
  if(m_paths[i]->type == 2){
   path = m_paths[i]->path + delim + kgmString(id, strlen(id));
   if(kgmIGame::getGame()->getSystem()->isFile(path) && file.open(path, kgmFile::Read)){
    m.alloc(file.length());
    file.read(m.data(), file.length());
    file.close();
    return true;
   }
  }else if(m_paths[i]->type == 1){
   if(m_paths[i]->archive.copy(id, m)){
    return true;
   }
  }
 }

 //fprintf(stderr, "\nCan't load file: %s", id);
 return false;
}

kgmPicture* kgmGameResources::getPicture(char* id){
 for(int i = 0; i < m_resources.size(); i++){
  if(!strcmp(m_resources[i]->m_id, id)){
   m_resources[i]->increment();
   return (kgmPicture*)m_resources[i];
  }
 }

 kgmMemory<char> mem;
 kgmPicture* picture = 0;
 if(!getFile(id, mem))
   return 0;
 picture = m_tools.genPicture(mem);
 if(picture){
   picture->m_id = id;
   m_resources.add(picture);
 }
 return picture;
}

kgmTexture* kgmGameResources::getTexture(char* id){
 int i = 0;
 for(i = 0; i < m_resources.size(); i++){
  if(!strcmp(m_resources[i]->m_id, id)){
   m_resources[i]->increment();
   return (kgmTexture*)m_resources[i];
  }
 }
 kgmTexture* texture = 0;
 kgmMemory<char> mem;
 if(!getFile(id, mem))
  return 0;
 texture = m_tools.genTexture(kgmIGame::getGame()->getGraphics(), mem);
 if(texture){
  texture->m_id = id;
  m_resources.add(texture);
 }
 return texture;
}

//Material
kgmMaterial* kgmGameResources::getMaterial(char* id){
  kgmMaterial* mtl = 0;
  for(int i = 0; i < m_resources.size(); i++){
    if(!strcmp(m_resources[i]->m_id, id) && (m_resources[i]->isClass(kgmMaterial::Class))){
      m_resources[i]->increment();
      return (kgmMaterial*)m_resources[i];
    }
  }

  kgmMemory<char> mem;
  if(!getFile(id, mem))
    return 0;

  kgmXml xml(mem);
  mtl = m_tools.genMaterial(xml);

  if(mtl){
    mtl->m_id = id;
    m_resources.add(mtl);
  }
  return mtl;
}

//Shader
kgmShader* kgmGameResources::getShader(char* id){
 kgmCString ext;
 for(int i = 0; i < m_resources.size(); i++){
  if(!strcmp(m_resources[i]->m_id, id)){
   m_resources[i]->increment();
   return (kgmShader*)m_resources[i];
  }
 }
 kgmCString path, name, dir;
 name = id;
 kgmShader* shader = 0;
 kgmMemory<char> mem;
 if(getFile(name, mem)){
   kgmString s(mem.data(), mem.length());
   shader = m_tools.genShader(kgmIGame::getGame()->getGraphics(), s);
 }

 if(shader){
   shader->m_id = id;
   m_resources.add(shader);
#ifdef DEBUG
   fprintf(stderr, "\nShader: %s successfully loaded", (char*)id);
#endif
 }
 return shader;
}

kgmAnimation* kgmGameResources::getAnimation(char* id){
 for(int i = 0; i < m_resources.size(); i++){
  if(!strcmp(m_resources[i]->m_id, id)){
   m_resources[i]->increment();
   return (kgmAnimation*)m_resources[i];
  }
 }
 kgmAnimation* anim = 0;
 kgmMemory<char> mem;
 if(!getFile(id, mem))
	 return 0;

 kgmXml xml(mem);
 if(!xml.m_node){
	 return 0;
 }

 //anim = m_tools.genAnimation(mem);
 anim = m_tools.genAnimation(xml);

 if(anim){
  anim->m_id = id;
  m_resources.add(anim);
 }
 return anim;
}

kgmSound* kgmGameResources::getSound(char* id){
 for(int i = 0; i < m_resources.size(); i++){
  if(!strcmp(m_resources[i]->m_id, id)){
   m_resources[i]->increment();
   return (kgmSound*)m_resources[i];
  }
 }
 kgmSound* snd = 0;
 kgmMemory<char> mem;
 if(!getFile(id, mem))
  return 0;
 snd = m_tools.genSound(kgmIGame::getGame()->getAudio(), mem);
 if(snd){
  snd->m_id = id;
  m_resources.add(snd);
 }
 return snd;
}

kgmMesh* kgmGameResources::getMesh(char* id){
 for(int i = 0; i < m_resources.size(); i++){
  if(!strcmp(m_resources[i]->m_id, id)){
   m_resources[i]->increment();
   return (kgmMesh*)m_resources[i];
  }
 }
 kgmMesh* mesh = 0;
 kgmMemory<char> mem;
 if(!getFile(id, mem))
	 return 0;

 kgmXml xml(mem);
 if(!xml.m_node){
	 return 0;
 }

 //mesh = m_tools.genMesh(mem);
 mesh = m_tools.genMesh(xml);

 if(mesh){
  mesh->m_id = id;
  m_resources.add(mesh);
 }

 return mesh;
}

kgmSkeleton* kgmGameResources::getSkeleton(char* id){
 for(int i = 0; i < m_resources.size(); i++){
  if(!strcmp(id, m_resources[i]->m_id)){
   m_resources[i]->increment();
   return (kgmSkeleton*)m_resources[i];
  }
 }
 kgmSkeleton* skel = 0;
 kgmMemory<char> mem;
 if(!getFile(id, mem))
  return 0;

 kgmXml xml(mem);
 if(!xml.m_node){
	 return 0;
 }

 //skel = m_tools.genSkeleton(mem);
 skel = m_tools.genSkeleton(xml);
 if(skel){
  skel->m_id = id;
  m_resources.add(skel);
 }
 return skel;
}

kgmFont* kgmGameResources::getFont(char* id, u32 r, u32 c){
 for(int i = 0; i < m_resources.size(); i++){
  if(!strcmp(id, m_resources[i]->m_id)){
   m_resources[i]->increment();
   return (kgmFont*)m_resources[i];
  }
 }
 kgmFont* font = 0;
 kgmMemory<char> mem;
 if(!getFile(id, mem))
	 return 0;
 font = m_tools.genFont(kgmIGame::getGame()->getGraphics(), 10, 10, r, c, mem);

 if(font){
  font->m_id = id;
  m_resources.add(font);
 }
 return font;
}

