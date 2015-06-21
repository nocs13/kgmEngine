#include "kgmGameResources.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "kgmGameTools.h"


#include "../kgmBase/kgmEnvironment.h"
#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmMedia/kgmSound.h"

#include "../kgmSystem/kgmSystem.h"

#include "kgmActor.h"

#ifdef ANDROID
  #include <android/asset_manager.h>
  #include <android/asset_manager_jni.h>
#endif

static char str_buf[256];

kgmGameResources::kgmGameResources(kgmIGC* gc, kgmIAudio* audio)
{
  m_gc    = gc;
  m_audio = audio;
}

kgmGameResources::~kgmGameResources()
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    kgmResource* r = m_resources[i];

    if(r)
    {
      if(r->isClass(kgmTexture::Class))
      {
        m_gc->gcFreeTexture(((kgmTexture*)r)->m_texture);
      }
      else if(r->isClass(kgmShader::Class))
      {
        kgmShader* sh = (kgmShader*)r;
        m_gc->gcFreeShader(sh->m_shader);
//        m_gc->gcFreeShader(((kgmShader*)r)->m_shader);
      }
      else if(r->isClass(kgmSound::Class))
      {
        m_audio->remove(((kgmSound*)r)->getSound());
      }
      if(r->isClass(kgmFont::Class))
      {
        m_gc->gcFreeTexture(((kgmFont*)r)->m_texture);
      }

      ((kgmObject*)r)->release();
    }
  }

  m_resources.clear();

  for(int i = 0; i < m_paths.size(); i++)
    delete m_paths[i];

  m_paths.clear();
}

void kgmGameResources::add(kgmResource* r)
{
  bool exist = false;

  if(!r)
    return;

  for(int i = 0; i < m_resources.size(); i++)
  {
    if(r == m_resources[i])
    {
      exist = true;

      break;
    }
  }

  if(exist)
    return;

  m_resources.add(r);
}

void kgmGameResources::remove(kgmResource* r)
{
}

void kgmGameResources::addPath(kgmString s)
{
  Path* path = 0;

  if(kgmSystem::isDirectory(s))
  {
    path = new Path();
    path->type = 2;
    path->path = s;
  }
  else if(kgmSystem::isFile(s))
  {
    path = new Path();
    path->type = 1;
    path->path = s;
    path->archive.open(s);
  }

  if(path)
    m_paths.add(path);
}

bool kgmGameResources::getFile(char* id, kgmMemory<u8>& m)
{
  kgmString path;
  bool  res = false;
  int   i = 0;

#ifdef WIN32
  const kgmString delim((const char*)"\\", 1);
#else
  const kgmString delim((const char*)"/", 1);
#endif

#ifdef ANDROID

#ifdef DEBUG
  kgm_log() << "\nkgmEngine android loading file " << id << "\n";
#endif

  AAsset* asset = AAssetManager_open(kgm_android_getAssetManager(), (const char *) id, AASSET_MODE_UNKNOWN);

  if (NULL == asset)
  {
#ifdef DEBUG
    kgmLog::log("_ASSET_NOT_FOUND_");
#endif

    return false;
  }

  long size = AAsset_getLength(asset);
  m.alloc(size);
  AAsset_read (asset, m.data(), size);
#ifdef DEBUG
  kgm_log() << "\nkgmEngine android file size: " << (s32)size << "\n";
#endif
  AAsset_close(asset);

  return true;

#else

  for(i = 0; i < m_paths.size(); i++)
  {
    kgmFile file;

    if(m_paths[i]->type == 2)
    {
      path = m_paths[i]->path + delim + kgmString(id, strlen(id));

      if(kgmIGame::getGame()->getSystem()->isFile(path) && file.open(path, kgmFile::Read))
      {
        m.alloc(file.length());
        file.read(m.data(), file.length());
        file.close();

        return true;
      }
    }
    else if(m_paths[i]->type == 1)
    {
      if(m_paths[i]->archive.copy(id, m))
      {
        return true;
      }
    }
  }

#endif

#ifdef DEBUG
  kgm_log() << "\nCan't load file: " << id << "\n";
#endif

  return false;
}

kgmPicture* kgmGameResources::getPicture(char* id)
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(m_resources[i]->m_id, id))
    {
      m_resources[i]->increment();

      return (kgmPicture*)m_resources[i];
    }
  }

  kgmMemory<u8> mem;
  kgmPicture* picture = 0;

  if(!getFile(id, mem))
    return 0;

  picture = m_tools.genPicture(mem);

  if(picture)
  {
    picture->m_id = id;
    m_resources.add(picture);
  }

  return picture;
}

kgmTexture* kgmGameResources::getTexture(char* id)
{
  int i = 0;

  for(i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(m_resources[i]->m_id, id))
    {
      m_resources[i]->increment();

      return (kgmTexture*)m_resources[i];
    }
  }

  kgmTexture* texture = 0;
  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  texture = m_tools.genTexture(m_gc, mem);

  if(texture && texture->m_texture)
  {
#ifdef DEBUG
      kgm_log() << "texture generated";
#endif
    texture->m_id = id;
    m_resources.add(texture);
  }
  else if(texture)
  {
#ifdef DEBUG
      kgm_log() << "texture generation failed";
#endif
    texture->release();

    texture = null;
  }

  return texture;
}

//Shader
kgmShader* kgmGameResources::getShader(char* id)
{
  kgmCString ext;

  for(int i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(m_resources[i]->m_id, id))
    {
      m_resources[i]->increment();

      return (kgmShader*)m_resources[i];
    }
  }

  kgmCString path, name, dir;
  name = id;
  kgmShader* shader = 0;
  kgmMemory<u8> mem;

#ifdef DEBUG
    kgm_log() << "Shader loading: " << id << "...\n";
#endif

  if(getFile(name, mem))
  {
    kgmString s((const char*)mem.data(), mem.length());
    shader = m_tools.genShader(m_gc, s);
  }

  if(shader)
  {
    shader->m_id = id;
    m_resources.add(shader);
    shader->increment();
#ifdef DEBUG
    kgm_log() << "Shader: " << id << "successfully loaded\n";
#endif
  }

  return shader;
}

kgmAnimation* kgmGameResources::getAnimation(char* id)
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(m_resources[i]->m_id, id))
    {
      m_resources[i]->increment();

      return (kgmAnimation*)m_resources[i];
    }
  }

  kgmAnimation* anim = 0;
  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  kgmXml xml(mem);

  if(!xml.m_node)
  {
    return 0;
  }

  //anim = m_tools.genAnimation(mem);
  anim = m_tools.genAnimation(xml);

  if(anim)
  {
    anim->m_id = id;
    m_resources.add(anim);
  }

  return anim;
}

kgmSound* kgmGameResources::getSound(char* id)
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(m_resources[i]->m_id, id))
    {
      m_resources[i]->increment();

      return (kgmSound*)m_resources[i];
    }
  }

  kgmSound* snd = 0;
  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  snd = m_tools.genSound(kgmIGame::getGame()->getAudio(), mem);

  if(snd)
  {
    snd->m_id = id;
    m_resources.add(snd);
  }

  return snd;
}

kgmMesh* kgmGameResources::getMesh(char* id)
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(m_resources[i]->m_id, id) && m_resources[i]->isClass(kgmMesh::Class))
    {
      m_resources[i]->increment();

      return (kgmMesh*)m_resources[i];
    }
  }

  kgmMesh* mesh = 0;
  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  kgmXml xml(mem);

  if(!xml.m_node)
  {
    return 0;
  }

  //mesh = m_tools.genMesh(mem);
  mesh = m_tools.genMesh(xml);

  if(mesh)
  {
    mesh->m_id = id;
    m_resources.add(mesh);
  }

  return mesh;
}

kgmSkeleton* kgmGameResources::getSkeleton(char* id)
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(id, m_resources[i]->m_id))
    {
      m_resources[i]->increment();

      return (kgmSkeleton*)m_resources[i];
    }
  }

  kgmSkeleton* skel = 0;
  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return null;

  kgmXml xml(mem);

  if(!xml.m_node)
    return null;

  //skel = m_tools.genSkeleton(mem);
  skel = m_tools.genSkeleton(xml);

  if(skel)
  {
    skel->m_id = id;
    m_resources.add(skel);
  }

  return skel;
}

kgmFont* kgmGameResources::getFont(char* id, u32 r, u32 c)
{
  int k = 0;

  for(int i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(id, m_resources[i]->m_id))
    {
      m_resources[i]->increment();

      return (kgmFont*)m_resources[i];
    }
  }

  kgmFont* font = 0;
  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  font = m_tools.genFont(m_gc, 10, 10, r, c, mem);

  if(font)
  {
    font->m_id = id;
    m_resources.add(font);
  }

  return font;
}

kgmShapeCollision* kgmGameResources::getShapeCollision(char* id)
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    if(!strcmp(id, m_resources[i]->m_id) && m_resources[i]->isClass(kgmShapeCollision::Class))
    {
      m_resources[i]->increment();

      return (kgmShapeCollision*)m_resources[i];
    }
  }

  kgmShapeCollision* shape = null;
  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return null;

  kgmXml xml(mem);

  if(!xml.m_node){
    return null;
  }

  shape = m_tools.genShapeCollision(xml);

  if(shape)
  {
    shape->m_id = id;
    m_resources.add(shape);
  }

  return shape;
}

