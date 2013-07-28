#include "kgmObject.h"
#include "kgmList.h"

static kgmList<kgmObject*> g_objects;

KGMOBJECT_IMPLEMENT(kgmObject, kgmObject)

void* kgmObject::operator new(size_t size)
{
 kgmObject* p = (kgmObject*)malloc(size);
 g_objects.push_back(p);

 return p;
}

void kgmObject::operator delete(void* p){
 int i;

 for(kgmList<kgmObject*>::iterator i = g_objects.begin(); i != g_objects.end(); i.next())
 {
   if((kgmObject*)p == (*i))
   {
     g_objects.erase(i);

     break;
   }
 }

 free(p);
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
  return g_objects.length();
}

bool kgmObject::isValid(kgmObject* o)
{
  for(kgmList<kgmObject*>::iterator i = g_objects.begin(); i != g_objects.end(); i++)
   if(o == *i)
     return true;

  return false;
}
