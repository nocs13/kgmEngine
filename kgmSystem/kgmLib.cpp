#include "kgmLib.h"

kgmLib::kgmLib(){
 handle = 0;
}
kgmLib::~kgmLib(){
 if(handle)
	 close();
}

bool kgmLib::open(char *p)
{
#ifdef WIN32
	handle = (void*)LoadLibrary(p);
#elif LINUX
	handle = dlopen(p,RTLD_NOW);
#endif
 return (handle)?(true):(false);
}

void kgmLib::close()
{
 if(!handle)
  return;
#ifdef WIN32
 FreeLibrary((HINSTANCE)handle);
#elif LINUX
 dlclose(handle);
#endif
 handle = 0;
}

void* kgmLib::get(char *sym)
{
#ifdef WIN32
	return (void*)GetProcAddress((HINSTANCE)handle,sym);
#endif
#ifdef LINUX
	return dlsym(handle, sym);
#endif
}

