#include "kgmObject.h"
#include "kgmLog.h"
#include "kgmArray.h"

#define ADDOBJS 128
static kgmArray<kgmObject*> g_objects;

void* kgmObject::kgm_object_alloc(size_t size)
{
  if (size < 1)
    return null;

  kgmObject* p = null;

  p = (kgmObject*) malloc(size);

  for (s32 i = 0; i < g_objects.length(); i++) {
    if (g_objects[i] == null) {
      g_objects[i] = p;

    #ifdef DEBUG
    fprintf(stderr, "kgmObject::kgm_object_alloc [%p %lld]\n", p, size);
    #endif

      return p;
    }
  }

  s32 len = g_objects.length();

  g_objects.realloc(len + ADDOBJS);

  void* pp = g_objects.data();

  for (s32 i = len; i < (len + ADDOBJS); i++)
    g_objects[i] = null;

  g_objects[len] = p;

  #ifdef DEBUG
  fprintf(stderr, "kgmObject::kgm_object_alloc [%p %lld]\n", p, size);
  #endif

  return p;
}

void kgmObject::kgm_object_free(void* p)
{
  if (!p)
    return;

  #ifdef DEBUG
  fprintf(stderr, "kgmObject::kgm_object_free [%p]\n", p);
  #endif

  for (s32 i = 0; i < g_objects.length(); i++) {
    if (g_objects[i] == p) {
      free(p);

      g_objects[i] = null;
    }
  }
}

void kgmObject::kgm_objects_cleanup()
{
#ifdef DEBUG
  kgm_log() << "kgmObject::kgm_objects_cleanup.\n";
#endif

  for (s32 i = 0; i < g_objects.length(); i++) {
    if (g_objects[i] != null) {
      delete g_objects[i];

      g_objects[i] = null;
    }
  }

  g_objects.clear();
}

kgmObject::kgmObject()
{
  increment();
}

kgmObject::kgmObject(const kgmObject& o)
{
  increment();
}

kgmObject::~kgmObject()
{
#ifdef DEBUG_X
    kgm_log() << "kgmObject::~kgmObject [" << (void*)this << "] class: [" << this->vClass() << "].\n";
#endif
}

bool kgmObject::isClass(const char* o)
{
  return !strcmp(this->vClass(), o);
}

void kgmObject::Release(kgmObject* o)
{
  if (o != nullptr)
    o->release();
}
