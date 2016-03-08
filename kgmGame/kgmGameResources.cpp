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
  clear(-1);

  for(int i = 0; i < m_paths.size(); i++)
    delete m_paths[i];

  m_paths.clear();
}

void kgmGameResources::add(kgmResource *r)
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    kgmResource* res = m_resources[i];

    if(r == res)
    {
      return;
    }
  }

  m_resources.add(r);
}

kgmResource* kgmGameResources::get(char* id)
{
  if(!id)
    return null;

  for(int i = 0; i < m_resources.size(); i++)
  {
    kgmResource* r = m_resources[i];

    if(!strcmp(r->m_id, id))
    {
      r->increment();

      return r;
    }
  }

  return null;
}

kgmResource* kgmGameResources::get(kgmString& id)
{
  return get((char *)id);
}

void kgmGameResources::clear(u32 group)
{
  for(int i = m_resources.size(); i > 0; i--)
  {
    kgmResource* r = m_resources[i - 1];

    if((group != -1) && (r->group() != group))
      continue;

    if(r->isClass(kgmTexture::Class))
    {
      m_gc->gcFreeTexture(((kgmTexture*)r)->m_texture);

      delete (kgmTexture*) r;
    }
    else if(r->isClass(kgmShader::Class))
    {
      m_gc->gcFreeShader(((kgmShader*)r)->m_shader);

      delete (kgmShader*) r;
    }
    else if(r->isClass(kgmSound::Class))
    {
      m_audio->remove(((kgmSound*)r)->getSound());

      delete (kgmSound*) r;
    }
    else if(r->isClass(kgmFont::Class))
    {
      m_gc->gcFreeTexture(((kgmFont*)r)->m_texture);

      delete (kgmFont*) r;
    }
    else if(r->isClass(kgmMesh::Class))
    {
      delete (kgmMesh*) r;
    }

    m_resources.erase(i - 1);
  }

  if(group == -1)
    m_resources.clear();
}

void kgmGameResources::remove(kgmResource* r)
{
  r->release();
}

bool kgmGameResources::exists(kgmResource* r)
{
  for(int i = 0; i < m_resources.size(); i++)
  {
    kgmResource* res = m_resources[i];

    if(r == res)
    {
      return true;
    }
  }

  return false;
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
  int   i = 0;

  if(!id)
    return false;

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
  kgm_log() << "kgmGameResources::getFile Cannot load file: " << id << "\n";
#endif

  return false;
}

kgmPicture* kgmGameResources::getPicture(char* id)
{
  kgmPicture* picture = (kgmPicture *)get(id);

  if(picture)
    return picture;

  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  picture = m_tools.genPicture(mem);

  if(picture)
  {
    picture->m_id = id;
    lock(picture);

    add(picture);
  }

  return picture;
}

kgmTexture* kgmGameResources::getTexture(char* id)
{
  kgmTexture* texture = (kgmTexture*)get(id);

  if(texture)
    return texture;

  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  texture = m_tools.genTexture(m_gc, mem);

  if(texture)
  {
    texture->m_id = id;
    lock(texture);

    add(texture);
  }

  return texture;
}

kgmShader* kgmGameResources::getShader(char* id)
{
  kgm_log() << "Resources: get shader " << id << "\n";

  kgmShader* shader = (kgmShader*)get(id);

  if(shader)
    return shader;

  kgmCString name;
  name = id;
  kgmMemory<u8> mem;

  if(getFile(name, mem))
  {
    kgmString s((const char*)mem.data(), mem.length());
    shader = m_tools.genShader(m_gc, s);
  }

  if(shader)
  {
    shader->m_id = id;
    lock(shader);

    add(shader);
  }

  return shader;
}

kgmAnimation* kgmGameResources::getAnimation(char* id)
{
  kgmAnimation* animation = (kgmAnimation*)get(id);

  if(animation)
    return animation;

  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  kgmXml xml(mem);

  if(!xml.m_node)
  {
    return 0;
  }

  animation = m_tools.genAnimation(xml);

  if(animation)
  {
    animation->m_id = id;
    lock(animation);

    add(animation);
  }

  return animation;
}

kgmSound* kgmGameResources::getSound(char* id)
{
  kgmSound* sound = (kgmSound*)get(id);

  if(sound)
    return sound;

  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  sound = m_tools.genSound(kgmIGame::getGame()->getAudio(), mem);

  if(sound)
  {
    sound->m_id = id;
    lock(sound);

    add(sound);
  }

  return sound;
}

kgmMesh* kgmGameResources::getMesh(char* id)
{
  kgmMesh* mesh = (kgmMesh*)get(id);

  if(mesh)
    return mesh;

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
    lock(mesh);

    add(mesh);
  }

  return mesh;
}

kgmSkeleton* kgmGameResources::getSkeleton(char* id)
{
  kgmSkeleton* skeleton = (kgmSkeleton*)get(id);

  if(skeleton)
    return skeleton;

  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return null;

  kgmXml xml(mem);

  if(!xml.m_node)
    return null;

  skeleton = m_tools.genSkeleton(xml);

  if(skeleton)
  {
    skeleton->m_id = id;
    lock(skeleton);

    add(skeleton);
  }

  return skeleton;
}

kgmFont* kgmGameResources::getFont(char* id, u32 r, u32 c)
{
  kgmFont* font = (kgmFont*)get(id);

  if(font)
    return font;

  kgmMemory<u8> mem;

  if(!getFile(id, mem))
    return 0;

  font = m_tools.genFont(m_gc, 10, 10, r, c, mem);

  if(font)
  {
    font->m_id = id;
    lock(font);

    add(font);
  }

  return font;
}

kgmShapeCollision* kgmGameResources::getShapeCollision(char* id)
{
  kgmShapeCollision* shape = (kgmShapeCollision*)get(id);

  if(shape)
    return shape;

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
    lock(shape);

    add(shape);
  }

  return shape;
}
