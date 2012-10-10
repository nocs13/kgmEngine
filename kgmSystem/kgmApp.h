#pragma once

class kgmEvent;
class kgmWindow;
class kgmEnvironment;

class kgmApp{
private:
 static kgmApp* m_app;

public:
 kgmApp();
 ~kgmApp();

 virtual void main();

 static kgmApp*  application(){
  return kgmApp::m_app;
 }
};

