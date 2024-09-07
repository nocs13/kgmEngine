#pragma once

#include "../kgmBase/kgmIApp.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"

#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#endif

class kgmEvent;
class kgmWindow;
class kgmEnvironment;

//void kgm_app_abort();

class kgmISettings;
class kgmIResources;

class kgmApp: public kgmIApp
{
private:
  static kgmApp* m_app;

  static kgmString m_execPath;

  void* m_mainWindow;


  kgmISettings*  m_settings;
  kgmIResources* m_resources;
  
private:
  void* operator new(size_t s);

public:
  kgmApp();
  virtual ~kgmApp();

  s32 exec(s32 argc, s8 **argv);


  void setMainWindow(void* w)
  {
    if(w) m_mainWindow = w;
  }

  void* getMainWindow()
  {
    return m_mainWindow;
  }

  kgmIResources* getResources()
  {
    return m_resources;
  }

  kgmISettings* getSettings()
  {
    return m_settings;
  }

  //static
  static kgmApp*  application()
  {
    return kgmApp::m_app;
  }

  static void exit(s32 e);

  static void Abort();
};
