#include "kgmResources.h"
#include "kgmIGame.h"
#include "kgmGameApp.h"
#include "kgmGameTools.h"

#include "../kgmBase/kgmFile.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmMedia/kgmSound.h"

#include "../kgmSystem/kgmSystem.h"
#include "../kgmSystem/kgmProcess.h"

#ifdef ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

#define RMESH      "meshes"
#define RIMAGE     "images"
#define RSOUND     "sounds"
#define RSHADER    "shaders"
#define RCONVEX    "convexes"
#define RMATERIAL  "materials"
#define RSKELETON  "skeletons"
#define RANIMATION "animations"

kgmResources::kgmResources(kgmIGC *gc, kgmIAudio *audio)
{
  m_gc = gc;
  m_audio = audio;
}

kgmResources::~kgmResources()
{
  clear();

  for (int i = 0; i < m_paths.size(); i++)
    delete m_paths[i];

  m_paths.clear();
}

void kgmResources::add(kgmResource *r)
{
  if (!r)
    return;

  for (kgmList<kgmResource *>::iterator i = m_resources.begin(); i != m_resources.end(); i++)
    if (r == (*i))
      return;

  m_resources.add(r);

  // kgmTab<const char*, kgmResource*>::iterator i = m_resources.get(r->id());

  // if (i.isEnd())
  //   m_resources.set(r->id(), r);
}

kgmResource *kgmResources::get(const char *id)
{
  if (!id)
    return null;

  for (kgmList<kgmResource *>::iterator i = m_resources.begin(); i != m_resources.end(); i++)
    if ((*i)->id() == id)
      return (*i);

  return null;

  // kgmTab<const char*, kgmResource*>::iterator i = m_resources.get(id);

  // if (i.isEnd())
  //   return null;

  // return (*i);
}

kgmResource *kgmResources::get(kgmString &id)
{
  return get((char *)id);
}

void kgmResources::reset()
{
}

void kgmResources::clear()
{
  kgmList<kgmResource *>::iterator i;

  for (i = m_resources.begin(); i != m_resources.end(); i++)
  {
    kgmResource *r = (*i);

    if (r->references() > 1)
      continue;

    if (r->isClass(kgmTexture::cClass()))
    {
      m_gc->gcFreeTexture(((kgmTexture *)r)->texture());

      delete (kgmTexture *)r;
    }
    else if (r->isClass(kgmShader::cClass()))
    {
      m_gc->gcFreeShader(((kgmShader *)r)->m_shader);

      delete (kgmShader *)r;
    }
    else if (r->isClass(kgmSound::cClass()))
    {
      m_audio->remove(((kgmSound *)r)->getSound());

      delete (kgmSound *)r;
    }
    else if (r->isClass(kgmFont::cClass()))
    {
      gchandle tn = ((kgmFont *)r)->texture(12);
      gchandle ts = ((kgmFont *)r)->texture(10);
      gchandle tl = ((kgmFont *)r)->texture(22);

      if (tn)
        m_gc->gcFreeTexture(tn);

      if (ts && ts != tn)
        m_gc->gcFreeTexture(ts);

      if (tl && tl != tn && tl != ts)
        m_gc->gcFreeTexture(tl);

      tn = tl = ts = null;

      delete (kgmFont *)r;
    }
    else if (r->isClass(kgmMesh::cClass()))
    {
      delete (kgmMesh *)r;
    }
  }

  m_resources.clear();
}

void kgmResources::remove(kgmResource *r)
{
  r->release();
}

bool kgmResources::exists(kgmResource *r)
{
  if (!r)
    return false;

  if (get(r->id()) == null)
    return false;

  // kgmTab<const char*, kgmResource*>::iterator i = m_resources.get(r->id());

  // if (i.isEnd())
  //   return false;

  return true;
}

bool kgmResources::fileFromData(kgmArray<u8> &mem, kgmString path)
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

bool kgmResources::dataFromFile(kgmString path, kgmArray<u8> &mem)
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

void kgmResources::addPath(kgmString s)
{
  Path *path = 0;

  if (kgmSystem::isDirectory(s))
  {
    path = new Path();
    path->type = 2;
    path->path = s;
  }
  else if (kgmSystem::isFile(s))
  {
    path = new Path();
    path->type = 1;
    path->path = s;
    path->archive.open(s);
  }

  if (path)
  {
    m_paths.add(path);

    kgm_log() << __FUNCTION__ << ": add path is " << path->path.data() << "\n";
  }
}

bool kgmResources::getFile(const char *id, kgmArray<u8> &m)
{
  return getRFile(id, null, m);
}

bool kgmResources::getRFile(const char *id, const char *type, kgmArray<u8> &m)
{
  kgmString path;

  if (!id)
    return false;

#ifdef WIN32
  const kgmString delim((const char *)"\\", 1);
#else
  const kgmString delim((const char *)"/", 1);
#endif

#ifdef ANDROID

  AAsset *asset = AAssetManager_open(kgm_android_getAssetManager(), (const char *)id, AASSET_MODE_UNKNOWN);

  if (NULL == asset)
  {
    kgmLog::log("_ASSET_NOT_FOUND_");

    return false;
  }

  long size = AAsset_getLength(asset);
  m.alloc(size);
  AAsset_read(asset, m.data(), size);

#ifdef DEBUG
  kgm_log() << "\nkgmEngine android file size: " << (s32)size << "\n";
#endif

  AAsset_close(asset);

  return true;

#else

  for (s32 i = 0; i < m_paths.size(); i++)
  {
    kgmFile file;

    kgm_log() << __FUNCTION__ << ": path type is " << m_paths[i]->type << "\n";
    kgm_log() << __FUNCTION__ << ": path is " << m_paths[i]->path.data() << "\n";

    if (m_paths[i]->type == 2)
    {
      if (type)
        path = m_paths[i]->path + delim + kgmString(type, strlen(type)) + delim + kgmString(id, strlen(id));
      else
        path = m_paths[i]->path + delim + kgmString(id, strlen(id));

      kgm_log() << __FUNCTION__ << ": path is " << m_paths[i]->path.data() << "\n";

      if (kgmSystem::isFile(path) && file.open(path, kgmFile::Read))
      {
        m.alloc(file.length());
        file.read(m.data(), file.length());
        file.close();

        return true;
      }
    }
    else if (m_paths[i]->type == 1)
    {
      kgmString sid;

      if (type)
        sid = kgmString(type) + kgmString("/") + kgmString(id);
      else
        sid = kgmString(id);

      if (m_paths[i]->archive.copy(sid, m))
      {
        return true;
      }
    }
  }

#endif

#ifdef DEBUG
  kgm_log() << "kgmResources::getFile Cannot load file: " << id << "\n";
#endif

  return false;
}

kgmPicture *kgmResources::getPicture(const char *id)
{
#ifdef DEBUG
  kgm_log() << "kgmResources::getPicture: " << id << "\n";
#endif

  kgmPicture *picture = (kgmPicture *)get(id);

  if (picture)
    return picture;

  kgmArray<u8> mem;

  if (!getRFile(id, RIMAGE, mem))
    return 0;

  picture = m_tools.genPicture(mem);

  if (picture)
  {
    picture->m_id = id;
    lock(picture);

    add(picture);
  }

  return picture;
}

kgmTexture *kgmResources::getTexture(const char *id)
{
#ifdef DEBUG
  kgm_log() << "kgmResources::getTexture: " << id << "\n";
#endif

  kgmTexture *texture = (kgmTexture *)get(id);

  if (texture)
    return texture;

  kgmArray<u8> mem;

  if (!getRFile(id, RIMAGE, mem))
    return 0;

  texture = m_tools.genTexture(m_gc, mem);

  if (texture)
  {
    texture->m_id = id;
    lock(texture);

    add(texture);
  }

  return texture;
}

kgmShader *kgmResources::getShader(const char *id)
{
#ifdef DEBUG
  kgm_log() << "kgmResources::getShader: " << id << "\n";
#endif

  kgmShader *shader = (kgmShader *)get(id);

  if (shader)
    return shader;

  kgmString sid = id;

  kgmCString name;

  name = id;

  if (m_gc->gcGetBase() == gc_vulkan)
  {
    kgmString prev;

#if defined(_WIN32)
    prev = ".win";
#elif defined(__unix__)
    prev = ".nix";
#endif

    /*
    kgmArray<u8> mem;
    kgmString kid = kgmString(id) + ".vk";

    if (getRFile(kid, RSHADER, mem))
    {
      kgmString s((const char *)mem.data(), mem.length());

      shader = m_tools.genShader(m_gc, s);
    }
    else
    {
      kgm_log() << "Shader vulkan not loaded for " << kid.data() << "\n";
    }
    */

    if (!shader)
    {
      kgmArray<u8> vmem, fmem;

      kgmString vid = kgmString(id) + prev + ".vspv";
      kgmString fid = kgmString(id) + prev + ".fspv";

      if (getRFile(vid, RSHADER, vmem) && getRFile(fid, RSHADER, fmem))
      {
        shader = m_tools.genShader(m_gc, vmem, fmem);
      }
      else
      {
#ifdef DEBUG
        kgm_log() << "Shader vulkan vertex for " << vid.data() << "\n";
        kgm_log() << "Shader vulkan fragment for " << fid.data() << "\n";
        kgm_log() << "Shader vulkan not loaded for " << id << "\n";
#endif
      }
    }
  }
  else
  {
    kgmArray<u8> mem;

    if (getRFile(name, RSHADER, mem))
    {
      kgmString s((const char *)mem.data(), mem.length());

      shader = m_tools.genShader(m_gc, s);
    }
  }

  if (shader)
  {
    shader->m_id = id;
    lock(shader);
    add(shader);
    unlock(shader);
  }

  return shader;
}

kgmMaterial *kgmResources::getMaterial(const char *id)
{
  kgm_log() << "Resources: get shader " << id << "\n";

  kgmMaterial *material = (kgmMaterial *)get(id);

  if (material)
    return material;

  kgmCString name;
  name = kgmString(id) + ".mtl";
  kgmArray<u8> mem;

  if (getRFile(name, RMATERIAL, mem))
  {
    kgmXml xml(mem);

    if (!xml.m_node)
    {
      return null;
    }

    material = m_tools.genMaterial(xml);
  }

  if (material)
  {
    material->m_id = id;
    lock(material);

    add(material);
  }

  return material;
}

kgmAnimation *kgmResources::getAnimation(const char *id)
{
  kgmAnimation *animation = (kgmAnimation *)get(id);

  if (animation)
    return animation;

  kgmArray<u8> mem;

  if (!getRFile(id, RANIMATION, mem))
    return 0;

  kgmXml xml(mem);

  if (!xml.m_node)
  {
    return 0;
  }

  animation = m_tools.genAnimation(xml);

  if (animation)
  {
    animation->m_id = id;
    lock(animation);

    add(animation);
  }

  return animation;
}

kgmSound *kgmResources::getSound(const char *id)
{
  kgmSound *sound = (kgmSound *)get(id);

  if (sound)
    return sound;

  kgmArray<u8> mem;

  if (!getRFile(id, RSOUND, mem))
    return 0;

  sound = m_tools.genSound(m_audio, mem);

  if (sound)
  {
    sound->m_id = id;
    lock(sound);

    add(sound);
  }

  return sound;
}

kgmMesh *kgmResources::getMesh(const char *id)
{
  kgmMesh *mesh = (kgmMesh *)get(id);

  if (mesh)
    return mesh;

  kgmArray<u8> mem;

  if (!getRFile(id, RMESH, mem))
    return 0;

  kgmXml xml(mem);

  if (!xml.m_node)
  {
    return 0;
  }

  // mesh = m_tools.genMesh(mem);
  mesh = m_tools.genMesh(xml);

  if (mesh)
  {
    mesh->m_id = id;
    lock(mesh);

    add(mesh);
  }

  return mesh;
}

kgmSkeleton *kgmResources::getSkeleton(const char *id)
{
  kgmSkeleton *skeleton = (kgmSkeleton *)get(id);

  if (skeleton)
    return skeleton;

  kgmArray<u8> mem;

  if (!getRFile(id, RSKELETON, mem))
    return null;

  kgmXml xml(mem);

  if (!xml.m_node)
    return null;

  skeleton = m_tools.genSkeleton(xml);

  if (skeleton)
  {
    skeleton->m_id = id;
    lock(skeleton);

    add(skeleton);
  }

  return skeleton;
}

kgmFont *kgmResources::getFont(const char *id, u32 r, u32 c)
{
  kgmFont *font = (kgmFont *)get(id);

  if (font)
    return font;

  kgmArray<u8> mem;

  if (!getRFile(id, RIMAGE, mem))
    return 0;

  font = m_tools.genFont(m_gc, 10, 10, r, c, mem);

  if (font)
  {
    font->m_id = id;
    lock(font);

    add(font);
  }

  return font;
}

kgmShapeCollision *kgmResources::getShapeCollision(const char *id)
{
  kgmShapeCollision *shape = (kgmShapeCollision *)get(id);

  if (shape)
    return shape;

  kgmArray<u8> mem;

  if (!getRFile(id, RCONVEX, mem))
    return null;

  kgmXml xml(mem);

  if (!xml.m_node)
  {
    return null;
  }

  shape = m_tools.genShapeCollision(xml);

  if (shape)
  {
    shape->m_id = id;
    lock(shape);

    add(shape);
  }

  return shape;
}
