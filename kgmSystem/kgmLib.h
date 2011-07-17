#pragma once 
//dinamically loaded Shared object, .so or .dll

#ifdef WIN32
 #include <windows.h>
#endif
#ifdef LINUX
 #include <dlfcn.h>
#endif


class kgmLib{
 void* handle;
public:
 kgmLib();
 ~kgmLib();
 bool open(char *p);
 void close();
 void* get(char *sym);
};


