#pragma once

class kgmEvent;
class kgmWindow;
class kgmEnvironment;

class kgmApp{
private:
  static kgmApp* m_app;

  void* m_mainWindow;

public:
  kgmApp();
  ~kgmApp();

  virtual int main(int argc, char **argv);

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
