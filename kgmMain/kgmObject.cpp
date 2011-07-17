#include "kgmObject.h"
#include <vector>

static std::vector<kgmObject*> g_objects;

KGMOBJECT_IMPLEMENT(kgmObject, kgmObject)

void* kgmObject::operator new(unsigned int size){
 kgmObject* p = (kgmObject*)malloc(size);
 g_objects.push_back(p);
 return p;
}

void kgmObject::operator delete(void* p){
 int i;
 for(i = 0; i < g_objects.size(); i++)
  if((kgmObject*)p == g_objects[i])
   g_objects.erase(g_objects.begin() + i);
 free(p);
}

void kgmObject::releaseObjects(void)
{
 int i;
 for(i = 0; i < g_objects.size(); i++)
  delete g_objects[i];
 g_objects.clear();
}

