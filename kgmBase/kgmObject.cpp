#include "kgmObject.h"
#include "kgmList.h"
#include "kgmTab.h"
#include "kgmLog.h"

static kgmList<kgmObject*> g_objects;
static kgmTab<void*, u32>  g_pointers;

KGMOBJECT_IMPLEMENT(kgmObject, kgmObject)

void* kgmObject::operator new(size_t size)
{
  kgmObject* p = (kgmObject*)malloc(size);
  g_objects.push_back(p);
  //kgm_log() << "Add memory: " << (s32)p << "\n";
  return p;
}

void kgmObject::operator delete(void* p){
  bool is = false;

  for(kgmList<kgmObject*>::iterator i = g_objects.begin(); i != g_objects.end(); i.next())
  {
    if((kgmObject*)p == (*i))
    {
      i = g_objects.erase(i);
      is = true;

      break;
    }
  }

  if(is)
  {
    free(p);

    //kgm_log() << "Free memory: " << (s32)p << "\n";
  }
}

void kgmObject::releaseObjects(void)
{
  int i;
  for(i = 0; i < g_objects.size(); i++)
    delete g_objects[i];
  g_objects.clear();
}

int kgmObject::objectCount()
{
#ifdef DEBUG
  int c = 0;
  for(kgmList<kgmObject*>::iterator i = g_objects.begin(); i != g_objects.end(); i.next())
  {
    kgmObject* o = (*i);
    c++;
  }
#endif
  return g_objects.length();
}

bool kgmObject::isValid(kgmObject* o)
{
  for(kgmList<kgmObject*>::iterator i = g_objects.begin(); i != g_objects.end(); i.next())
    if(o == (*i))
      return true;

  return false;
}

#ifdef DEBUG
void kgmObject::listObjects()
{
  int k = 0;

  for(kgmList<kgmObject*>::iterator i = g_objects.begin(); i != g_objects.end(); i.next())
  {
    kgmObject* o = *i;
    //const char* nclass = o->runtime().nClass;
    k++;
  }
}
#endif

inline void* kgm_alloc(size_t size)
{
  void* p = malloc(size);

  if(p == null)
    return null;

  g_pointers.add(p, 1);

  return p;
}

inline void  kgm_free(void* p)
{
  for(kgmTab<void*,u32>::iterator i = g_pointers.begin(); i != g_pointers.end(); ++i)
  {
    if(i.key() == p)
    {
      i.value()--;

      if(i.value() < 1)
      {
        free(p);

        g_pointers.erase(i);
      }

      break;
    }
  }
}

inline void kgm_assign(void** src, void** dst)
{
  if(*src == null)
    return;

  for(kgmTab<void*,u32>::iterator i = g_pointers.begin(); i != g_pointers.end(); ++i)
  {
    if(i.key() == *src)
    {
      i.value()++;

      *dst = *src;

      break;
    }
  }
}
