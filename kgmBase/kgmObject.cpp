#include "kgmObject.h"
#include "kgmList.h"
#include "kgmTab.h"
#include "kgmLog.h"

//static kgmList<kgmObject*> g_objects;
//static kgmTab<void*, u32>  g_pointers;

KGMOBJECT_IMPLEMENT(kgmObject, kgmObject)

/*void* kgmObject::operator new(size_t size)
{
  kgmObject* p = (kgmObject*)malloc(size);
  g_objects.push_back(p);
  //kgm_log() << "Add memory: " << (s32)p << "\n";
  return p;
}

void kgmObject::operator delete(void* p)
{
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
}*/
