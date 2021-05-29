#include "kgmLib.h"
#include "../kgmBase/kgmTypes.h"
#include "../kgmBase/kgmLog.h"

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

  if (!handle)
    kgm_log() << "Cannot load dll [" << p << "], error is " << (s32) GetLastError() << "\n";
#else
  handle = dlopen(p, RTLD_LAZY);
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

bool kgmLib::active()
{
  return (handle)?(true):(false);
}
