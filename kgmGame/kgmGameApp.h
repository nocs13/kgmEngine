#pragma once
#include "kgmIGame.h"
#include "../kgmBase/kgmEvent.h"
#include "../kgmSystem/kgmApp.h"

class kgmGameApp: public kgmApp
{
protected:
  kgmIGame*  m_game;

public:
  virtual ~kgmGameApp(){

  }

  kgmIGame* game(){
    return m_game;
  }

  static kgmGameApp* gameApplication(){
    return (kgmGameApp*)kgmApp::application();
  }

#ifdef ANDROID
  bool android_msAbsolute;

  void android_quit()
  {

  }

  void android_idle()
  {

  }

  void android_input(kgmEvent::Event* e)
  {

  }

  void android_resize(int, int)
  {

  }

  void android_close()
  {

  }

  void android_release()
  {

  }

  bool android_msabsolute()
  {
    return android_msAbsolute;
  }
#endif
};

#ifdef ANDROID
  //initiate exit from program
  extern void       kgm_android_exit();
  //create game exempliar
  extern kgmIGame*  kgm_android_init_game();
  //create game exempliar
  extern bool  kgm_android_init_app();
#endif
