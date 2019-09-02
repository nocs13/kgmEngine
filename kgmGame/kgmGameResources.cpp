#include "kgmGameResources.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "kgmGameTools.h"

#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmMedia/kgmSound.h"

#include "../kgmSystem/kgmSystem.h"
#include "../kgmSystem/kgmProcess.h"

#include "kgmActor.h"

#ifdef ANDROID
  #include <android/asset_manager.h>
  #include <android/asset_manager_jni.h>
#endif

kgmGameResources::kgmGameResources(kgmIGC* gc, kgmIAudio* audio)
{
  m_gc    = gc;
  m_audio = audio;
}

kgmGameResources::~kgmGameResources()
{
  clear();

  for(int i = 0; i < m_paths.size(); i++)
    delete m_paths[i];

  m_paths.clear();
}

void kgmGameResources::add(kgmResource *r)
{
  if (!r)
    return;

  for (kgmList<kgmResource*>::iterator i = m_resources.begin(); !i.end(); ++i)
    if (r == (*i))
      return;

  m_resources.add(r);

  //kgmTab<const char*, kgmResource*>::iterator i = m_resources.get(r->id());

  //if (i.isEnd())
  //  m_resources.set(r->id(), r);
}

kgmResource* kgmGameResources::get(const char* id)
{
  if(!id)
    return null;

  for (kgmList<kgmResource*>::iterator i = m_resources.begin(); !i.end(); ++i)
    if ((*i)->id() == id)
      return (*i);

  return null;

  //kgmTab<const char*, kgmResource*>::iterator i = m_resources.get(id);

  //if (i.isEnd())
  //  return null;

  //return (*i);
}

kgmResource* kgmGameResources::get(kgmString& id)
{
  return get((char *)id);
}

void kgmGameResources::reset()
{

}

void kgmGameResources::clear()
{
  kgmList<kgmResource*>::iterator i;

  for(i = m_resources.begin(); !i.end(); ++i)
  {
    kgmResource* r = (*i);

    if (r->references() > 1)
      continue;

    if(r->isClass(kgmTexture::cClass()))
    {
      m_gc->gcFreeTexture(((kgmTexture*)r)->texture());

      delete (kgmTexture*) r;
    }
    else if(r->isClass(kgmShader::cClass()))
    {
      m_gc->gcFreeShader(((kgmShader*)r)->m_shader);

      delete (kgmShader*) r;
    }
    else if(r->isClass(kgmSound::cClass()))
    {
      m_audio->remove(((kgmSound*)r)->getSound());

      delete (kgmSound*) r;
    }
    else if(r->isClass(kgmFont::cClass()))
    {
      m_gc->gcFreeTexture(((kgmFont*)r)->texture());

      delete (kgmFont*) r;
    }
    else if(r->isClass(kgmMesh::cClass()))
    {
      delete (kgmMesh*) r;
    }

    //i = m_resources.erase(i);
  }

  m_resources.clear();
}

void kgmGameResources::remove(kgmResource* r)
{
  r->release();
}

bool kgmGameResources::exists(kgmResource* r)
{
  if (!r)
    return false;

  if (get(r->id()) == null)
    return false;

  //kgmTab<const char*, kgmResource*>::iterator i = m_resources.get(r->id());

  //if (i.isEnd())
  //  return false;

  return true;
}

bool kgmGameResources::fileFromData(kgmMemory<u8>& mem, kgmString path)
{
  if (!mem.data() || !path.data())
    return false;

  kgmFile file;

  if (!file.open(path, kgmFile::Write | kgmFile::Read | kgmFile::Create))
    return false;

  file.write(mem.data(), mem.length());

  file.close();

  return true;
}

bool kgmGameResources::dataFromFile(kgmString path, kgmMemory<u8> &mem)
{
  kgmFile file;

  if (!file.open(path, kgmFile::Read))
    return false;

  u32 size = file.length();

  if (size == -1)
  {
    file.close();

    return false;
  }

  mem.clear();

  mem.alloc(size);

  file.read(mem.data(), size);

  file.close();

  return true;
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

bool kgmGameResources::getFile(const char* id, kgmMemory<u8>& m)
{
  kgmString path;

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

  for(s32 i = 0; i < m_paths.size(); i++)
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

kgmPicture* kgmGameResources::getPicture(const char* id)
{
#ifdef DEBUG
  kgm_log() << "kgmGameResources::getPicture: " << id << "\n";
#endif

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

kgmTexture* kgmGameResources::getTexture(const char* id)
{
#ifdef DEBUG
  kgm_log() << "kgmGameResources::getTexture: " << id << "\n";
#endif

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

kgmShader* kgmGameResources::getShader(const char* id)
{
#ifdef DEBUG
  kgm_log() << "kgmGameResources::getShader: " << id << "\n";
#endif

  kgmShader* shader = (kgmShader*)get(id);

  if(shader)
    return shader;

  kgmString sid = id;

  kgmCString name;

  name = id;


  if (m_gc->gcGetBase() == gc_vulkan)
  {
    kgmString cmd;

    #ifdef WIN32
    cmd = "..\\Tools\\Spirv\\win32\\glslc.exe ";
    #else
    cmd = "../Tools/Spirv/nix/glslc";
    #endif

    kgmString tmp;

    kgmSystem::getTemporaryDirectory(tmp);

    #ifdef DEBUG
    kgm_log() << "Temporary folder is " << (char *) tmp << "\n";
    #endif

    kgmString args;

    kgmMemory<u8> data;

    if (!getFile(kgmString("shaders") + kgmSystem::getPathDelim() + id + ".vert", data))
      return null;

    if (!fileFromData(data, tmp + id + ".vert"))
      return null;

    data.clear();

    if (!getFile(kgmString("shaders") + kgmSystem::getPathDelim() + id + ".frag", data))
      return null;

    if (!fileFromData(data, tmp + id + ".frag"))
      return null;

    data.clear();

    kgmProcess proc;

    args = kgmString("-V ") + tmp + id + ".vert -o " + tmp + id + ".vert.spv";

    kgmString cline = cmd + " " + args;

    #ifdef DEBUG
    kgm_log() << "Command line is: " << (char *) cline << "\n";
    #endif

    //proc.run(cmd, args);
    proc.run(kgmString(), cline);
    proc.wait();

    args = kgmString("-V ") + tmp + id + ".frag -o " + tmp + id + ".frag.spv";

    cline = cmd + " " + args;

    //proc.run(cmd, args);
    proc.run(kgmString(), cline);
    proc.wait();

    kgmMemory<u8> vmem, fmem;

    kgmString vname = tmp + id + ".vert.spv";
    kgmString fname = tmp + id + ".frag.spv";

    if(dataFromFile(vname, vmem) && dataFromFile(fname, fmem))
    {
      shader = m_tools.genShader(m_gc, vmem, fmem);
    }
    else
    {
      #ifdef DEBUG
      kgm_log() << "Shader spirv not loaded for " << id << "\n";
      #endif
    }
  }
  else
  {
    kgmMemory<u8> mem;

    if(getFile(name, mem))
    {
      kgmString s((const char*)mem.data(), mem.length());
      shader = m_tools.genShader(m_gc, s);
    }
  }

  if(shader)
  {
    shader->m_id = id;
    lock(shader);

    add(shader);
  }

  return shader;
}

kgmMaterial* kgmGameResources::getMaterial(const char* id)
{
  kgm_log() << "Resources: get shader " << id << "\n";

  kgmMaterial* material = (kgmMaterial*)get(id);

  if(material)
    return material;

  kgmCString name;
  name = kgmString(id) + ".mtl";
  kgmMemory<u8> mem;

  if(getFile(name, mem))
  {
    kgmXml xml(mem);

    if(!xml.m_node)
    {
      return null;
    }

    material = m_tools.genMaterial(xml);
  }

  if(material)
  {
    material->m_id = id;
    lock(material);

    add(material);
  }

  return material;
}

kgmAnimation* kgmGameResources::getAnimation(const char* id)
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

kgmSound* kgmGameResources::getSound(const char* id)
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

kgmMesh* kgmGameResources::getMesh(const char* id)
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

kgmSkeleton* kgmGameResources::getSkeleton(const char* id)
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

kgmFont* kgmGameResources::getFont(const char* id, u32 r, u32 c)
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

kgmShapeCollision* kgmGameResources::getShapeCollision(const char* id)
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
