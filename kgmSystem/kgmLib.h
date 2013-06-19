#pragma once 
//dinamically loaded Shared object, .so or .dll

#ifdef WIN32
#include <windows.h>
#endif
#ifdef LINUX
#include <dlfcn.h>
#endif

#if defined(_MSC_VER)
  //  Ms Visual studio
  #define KGM_LIB_EXPORT __declspec(dllexport)
  #define KGM_LIB_IMPORT __declspec(dllimport)
#elif defined(_GCC)
  //  GCC
  #define KGM_LIB_EXPORT __attribute__((visibility("default")))
  #define KGM_LIB_IMPORT
#else
  //  do nothing and hope for the best?
  #define KGM_LIB_EXPORT
  #define KGM_LIB_IMPORT
  #pragma warning Unknown dynamic link import/export semantics.
#endif

class kgmLib
{
  void* handle;
public:
  kgmLib();
  ~kgmLib();

  bool  open(char *p);
  void  close();
  void* get(char *sym);
};


