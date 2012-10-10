#include "kgmObject.h"
#include <vector>

static std::vector<kgmObject*> g_objects;

KGMOBJECT_IMPLEMENT(kgmObject, kgmObject)

void* kgmObject::operator new(size_t size){
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

bool kgmObject::isValid(){
  for(int i = 0; i < g_objects.size(); i++)
   if((kgmObject*)this == g_objects[i])
     return true;

  return false;
}

void kgmObject::releaseObjects(void)
{
 int i;
 for(i = 0; i < g_objects.size(); i++)
  delete g_objects[i];
 g_objects.clear();
}

