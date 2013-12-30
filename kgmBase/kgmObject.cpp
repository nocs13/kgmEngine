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
#ifdef TEST
  for(kgmList<kgmObject*>::iterator i = g_objects.begin(); i != g_objects.end(); i.next())
  {
    kgmObject* o = (*i);
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

#ifdef TEST
void kgmObject::listObjects()
{
  int k = 0;

  for(kgmList<kgmObject*>::iterator i = g_objects.begin(); i != g_objects.end(); i.next())
  {
    kgmObject* o = *i;
    k++;
  }
}
#endif
