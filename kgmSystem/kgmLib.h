#pragma once 
//dinamically loaded Shared object, .so or .dll

#ifdef WIN32
#include <windows.h>
#else
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
#endif

#include "../kgmBase/kgmObject.h"

class kgmLib: public kgmObject
{
  KGM_OBJECT(kgmLib);

  void* handle;
public:
  kgmLib();
  ~kgmLib();

  bool  open(char *p);
  bool  active();
  void  close();
  void* get(char *sym);
};


