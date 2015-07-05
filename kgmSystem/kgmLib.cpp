#include "kgmLib.h"
#include "../kgmBase/kgmTypes.h"

KGMOBJECT_IMPLEMENT(kgmLib, kgmObject);


kgmLib::kgmLib()
{
  handle = 0;
}

kgmLib::~kgmLib()
{
  if(handle)
    close();
}

bool kgmLib::open(char *p)
{
#ifdef WIN32
  handle = (void*)LoadLibrary(p);
#else
  handle = dlopen(p, RTLD_NOW);
#endif

  return (handle)?(true):(false);
}

void kgmLib::close()
{
  if(!handle)
    return;

#ifdef WIN32
  FreeLibrary((HINSTANCE)handle);
#else
  dlclose(handle);
#endif

  handle = 0;
}

void* kgmLib::get(char *sym)
{
  if(sym == null)
    return null;

#ifdef WIN32
  return (void*)GetProcAddress((HINSTANCE)handle,sym);
#else
  return dlsym(handle, sym);
#endif

  return null;
}
