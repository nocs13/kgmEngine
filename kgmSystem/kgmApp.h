#pragma once

#include "../kgmBase/kgmIApp.h"

class kgmEvent;
class kgmWindow;
class kgmEnvironment;

void kgm_app_abort();

class kgmApp: public kgmIApp
{
private:
  static kgmApp* m_app;

  void* m_mainWindow;

public:
  kgmApp();
  ~kgmApp();

  s32 exec(s32 argc, s8 **argv);

  void setMainWindow(void* w)
  {
    if(w) m_mainWindow = w;
  }

  void* getMainWindow()
  {
    return m_mainWindow;
  }

  //static
  static kgmApp*  application()
  {
    return kgmApp::m_app;
  }
};
